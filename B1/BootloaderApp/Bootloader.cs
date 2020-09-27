/*******************************************************************************
* Copyright (C) 2020 Singular Software Limited.
* THIS SOFTWARE IS PROVIDED BY SIGNULAR SOFTWARE "AS IS" AND ANY EXPRESS OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY 
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/
using System;
using System.Collections.Generic;
using System.Drawing.Text;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BooloaderApp
{
    class Bootloader
    {
        private const UInt32 PIC32MZ_FLASH_START_ADDRESS = 0x9D000000;
        private const int PIC32MZ_FLASH_ERASE_BLOCK_SIZE = 16384;
        private const int CRC_SIZE = 4;

        private const int MAX_COMMAND_BUFFER_SIZE = (PIC32MZ_FLASH_ERASE_BLOCK_SIZE + 1 + 4 + CRC_SIZE/*command + address + crc*/) * 2;

        private enum BootloaderCommandFram
        {
            START  = 0x01,
            END    = 0x04,
            ESCAPE = 0x10,
        }

        private enum BootloaderCommand
        {
            HANDSHAKE_CMD = 55,
            VERSION_CMD,
            ADDRESS_CMD,
            PROGRAM_CMD,
            VERIFY_CMD,
            START_FIRMWARE_CMD,
            RESET_BOARD_CMD
        }
        private enum BootloaderResponse
        {
            HANDSHAKE_REP = BootloaderCommand.HANDSHAKE_CMD + 100,
            VERSION_REP,
            ADDRESS_REP,
            PROGRAM_REP,
            VERIFY_REP,
            START_FIRMWARE_REP,
            RESET_BOARD_REP,
            ERROR_REP
        }

        private readonly SerialPort _serialPort;
        private string _serialPortName;
        private readonly int _serialPortBaudRate;
        private byte[] _firmwware;
        private ProgressUpdate _progressUpdate = null;
        private ResultUpdate _resultUpdate = null;

        private readonly UInt32[] _crc32Table = {   0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
                                            0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
                                            0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
                                            0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
                                            0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
                                            0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
                                            0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
                                            0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
                                            0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
                                            0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
                                            0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
                                            0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
                                            0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
                                            0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
                                            0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
                                            0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
                                            0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
                                            0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
                                            0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
                                            0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
                                            0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
                                            0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
                                            0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
                                            0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
                                            0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
                                            0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
                                            0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
                                            0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
                                            0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
                                            0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
                                            0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
                                            0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D }; 
        private UInt32 CalculateCrc32(byte[] data, UInt32 startIndex, UInt32 lenthg)
        {
            UInt32 crc = 0xFFFFFFFF;
            UInt32 i = 0;

            while (lenthg-- != 0)
            {
                byte b = data[startIndex + i++];
                crc = _crc32Table[(crc ^ b) & 0xFF] ^ (crc >> 8);
            }
            
            crc ^= 0XFFFFFFFF;

            return crc;
        }
    

        private void Send(byte[] buffer)
        {
            SetCrc(buffer);

            int requestLength = 0;
            byte[] request = new byte[MAX_COMMAND_BUFFER_SIZE];

            request[requestLength++] = (byte)BootloaderCommandFram.START;
            //add escape char where needed
            for (int i = 0; i < buffer.Length; i++)
            {
                if ((buffer[i] == (byte)BootloaderCommandFram.START) ||
                    (buffer[i] == (byte)BootloaderCommandFram.END) ||
                    (buffer[i] == (byte)BootloaderCommandFram.ESCAPE))
                {
                    request[requestLength++] = (byte)BootloaderCommandFram.ESCAPE;//add escap 
                }
                request[requestLength++] = buffer[i];
            }
            request[requestLength++] = (byte)BootloaderCommandFram.END;

            // send
            try
            {
                _serialPort.DiscardInBuffer();
                _serialPort.DiscardOutBuffer();
                _serialPort.WriteTimeout = requestLength*5;
                _serialPort.Write(request, 0, requestLength);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
        private bool Receive(ref byte[] data)
        {
            bool result = false;

            try
            {
                Thread.Sleep(200);

                bool escape = false;
                byte[] response = new byte[0];
                response = new byte[512];
                int readCount = 0;

                while (_serialPort.BytesToRead > 0)
                {
                    readCount = _serialPort.Read(response, readCount, 512 - readCount);
                    Thread.Sleep(1);
                }

                Array.Resize(ref response, readCount);

                data = new byte[response.Length];
                int dataIndex = 0;

                for (int i = 0; i < response.Length && !result; i++)
                {
                    switch (response[i])
                    {
                        case (byte)BootloaderCommandFram.START: //check if start
                            if (escape)
                            {
                                data[dataIndex++] = response[i];// Received byte is not Start, it is data
                                escape = false;
                            }
                            else
                            {

                                dataIndex = 0; // Received byte is Start
                            }
                            break;

                        case (byte)BootloaderCommandFram.END://check if end
                            if (escape)
                            {
                                data[dataIndex++] = response[i];// Received byte is not End, it is data
                                escape = false;
                            }
                            else
                            {
                                // Received byte is end so cacluate CRC to verify the response
                                if (dataIndex > CRC_SIZE)//4
                                {
                                    UInt32 crc = (UInt32)(data[dataIndex - 4] & 0x000000FF);
                                    crc = (UInt32)(crc | (UInt32)((data[dataIndex - 3] << 8) & 0x0000FF00));
                                    crc = (UInt32)(crc | (UInt32)((data[dataIndex - 2] << 16) & 0x00FF0000));
                                    crc = (UInt32)(crc | (UInt32)((data[dataIndex - 1] << 24) & 0xFF000000));

                                    UInt32 receivedCrc = CalculateCrc32(data, 0, (UInt32)dataIndex - CRC_SIZE);

                                    result = receivedCrc == crc;
                                }
                            }
                            break;


                        case (byte)BootloaderCommandFram.ESCAPE: // Checvk if escape
                            if (escape)
                            {
                                data[dataIndex++] = response[i];// Received byte is not escape, it is data.
                                escape = false;
                            }
                            else
                            {
                                escape = true;// Received byte is escape
                            }
                            break;

                        default: // Data
                            data[dataIndex++] = response[i];
                            escape = false;
                            break;

                    }
                }

                Array.Resize(ref data, dataIndex - CRC_SIZE/*CRC*/);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
                Array.Resize(ref data, 0);
                result = false;
            }

            return result;
        }
        private void SetCrc(byte[] buffer)
        {
            UInt32 crc = CalculateCrc32(buffer, 0, (uint)buffer.Length - CRC_SIZE);
            Array.Copy(BitConverter.GetBytes(crc), 0, buffer, buffer.Length - CRC_SIZE, CRC_SIZE);
        }

        private void SendHandshakeCommand()
        {
            byte[] buffer = new byte[1 + CRC_SIZE];
            buffer[0] = (byte)BootloaderCommand.HANDSHAKE_CMD;
            Send(buffer);
        }
        private void SendReadBootloaderVersionCommand()
        {
            byte[] buffer = new byte[1 + CRC_SIZE];
            buffer[0] = (byte)BootloaderCommand.VERSION_CMD;
            Send(buffer);
        }
        private void SendStartApplicationCommand()
        {
            byte[] buffer = new byte[1 + CRC_SIZE];
            buffer[0] = (byte)BootloaderCommand.START_FIRMWARE_CMD;
            Send(buffer);
        }
        private void SendResetBoardCommand()
        {
            byte[] buffer = new byte[1 + CRC_SIZE];
            buffer[0] = (byte)BootloaderCommand.RESET_BOARD_CMD;
            Send(buffer);
        }

        private void SendFrimwareAddressCommand(UInt32 address, UInt32 lenth)
        {   
            byte[] startAddress = BitConverter.GetBytes(address);
            byte[] firmwareLength = BitConverter.GetBytes(lenth);

            byte[] buffer = new byte[8 + 1 + CRC_SIZE];
            buffer[0] = (byte)BootloaderCommand.ADDRESS_CMD;
            Array.Copy(startAddress, 0, buffer, 1, startAddress.Length);
            Array.Copy(firmwareLength, 0, buffer, 1+ startAddress.Length, firmwareLength.Length);
            Send(buffer);
        }
        private void SendProgramFlashCommand(byte[] data)
        {
            byte[] buffer = new byte[data.Length + 1 + CRC_SIZE];
            buffer[0] = (byte)BootloaderCommand.PROGRAM_CMD;
            Array.Copy(data, 0, buffer, 1, data.Length);
            Send(buffer);
        }
        private void SendVerifyCommand(UInt32 startAddress, UInt32 lebngth, UInt32 crc)
        {
            byte[] buffer = new byte[13 + CRC_SIZE];

            buffer[0] = (byte)BootloaderCommand.VERIFY_CMD;
            Array.Copy(BitConverter.GetBytes(startAddress), 0, buffer, 1, 4);
            Array.Copy(BitConverter.GetBytes(lebngth), 0, buffer, 5, 4);
            Array.Copy(BitConverter.GetBytes(crc), 0, buffer, 9, 4);

            Send(buffer);
        }
        private bool ReceiveHandshakeCommandResponse(ref byte[] data)
        {
            bool result = false;
            if (Receive(ref data))
            {
                result = data[0] == (byte)BootloaderResponse.HANDSHAKE_REP;
            }
            return result;
        }
        private bool ReceiveBootloaderVersionCommandResponse(ref byte[] data)
        {
            bool result = false;
            if (Receive(ref data))
            {
                result = data[0] == (byte)BootloaderResponse.VERSION_REP;
            }
            return result;
        }
        private bool ReceiveStartApplicationCommandResponse(ref byte[] data)
        {
            bool result = false;
            if (Receive(ref data))
            {
                result = data[0] == (byte)BootloaderResponse.START_FIRMWARE_REP;
            }
            return result;
        }
        private bool ReceiveResetBoardCommandResponse(ref byte[] data)
        {
            bool result = false;
            if (Receive(ref data))
            {
                result = data[0] == (byte)BootloaderResponse.RESET_BOARD_REP;
            }
            return result;
        }
        private bool ReceiveFirmwareAddressCommandResponse()
        {
            bool result = false;
            byte[] data = new byte[0];
            if (Receive(ref data))
            {
                result = data[0] == (byte)BootloaderResponse.ADDRESS_REP;
            }
            return result;
        }
        private bool ReceiveProgramFlashCommandResponse()
        {
            bool result = false;
            byte[] data = new byte[0];
            if (Receive(ref data))
            {
                result = data[0] == (byte)BootloaderResponse.PROGRAM_REP;
            }
            return result;
        }
        private bool ReceiveVerifyCommandResponse(ref byte[] data)
        {
            bool result = false;
            if (Receive(ref data))
            {
                result = data[0] == (byte)BootloaderResponse.VERIFY_REP;
            }
            return result;
        }

        private void ReportProgramUpdate(int minimum, int maximum, int value)
        {
            _progressUpdate?.Invoke(minimum, maximum, value);
        }
        private void ReportResultUpdate(bool success)
        {
            _resultUpdate?.Invoke(success);
        }
        private void ProgramFlash()//flash programming thread function
        {
            bool success = false;
            if (_firmwware.Length > 0)
            {
                //inform the bootloader of the address space to be program
                SendFrimwareAddressCommand(PIC32MZ_FLASH_START_ADDRESS, (UInt32)_firmwware.Length);
                success = ReceiveFirmwareAddressCommandResponse();

                int numberOfBlocks = _firmwware.Length / PIC32MZ_FLASH_ERASE_BLOCK_SIZE;
                int currentBlock = 0;
                ReportProgramUpdate(0, numberOfBlocks, currentBlock++);

                if (success)
                {
                    //send firmware 
                    byte[] block = new byte[PIC32MZ_FLASH_ERASE_BLOCK_SIZE + 4/*address*/];
                    UInt32 address = PIC32MZ_FLASH_START_ADDRESS;
                    UInt32 offset = 0;

                    while (offset < _firmwware.Length && success)
                    {
                        byte[] flashAddress = BitConverter.GetBytes((UInt32)address);
                        Array.Copy(flashAddress, 0, block, 0, flashAddress.Length);
                        Array.Copy(_firmwware, offset, block, flashAddress.Length, PIC32MZ_FLASH_ERASE_BLOCK_SIZE);
                        address += PIC32MZ_FLASH_ERASE_BLOCK_SIZE;
                        offset += PIC32MZ_FLASH_ERASE_BLOCK_SIZE;

                        SendProgramFlashCommand(block);
                        success = ReceiveProgramFlashCommandResponse();

                        ReportProgramUpdate(0, numberOfBlocks, currentBlock++);
                    }

                    if (success)
                    {
                        //verify
                        success = Verify(_firmwware);
                    }
                }
            }

            ReportResultUpdate(success);
        }

        public bool Open(string serialPortName)
        {
            bool result = false;
            try
            {
                result = _serialPort.IsOpen;

                _serialPortName = serialPortName;

                if (!result && _serialPortName != "")
                {
                    _serialPort.PortName = _serialPortName;
                    _serialPort.BaudRate = _serialPortBaudRate;
                    _serialPort.Parity = Parity.None;
                    _serialPort.DataBits = 8;
                    _serialPort.StopBits = StopBits.One;
                    _serialPort.ReadTimeout = 1000;
                    _serialPort.WriteTimeout = 1000;

                    _serialPort.Open();
                    _serialPort.DiscardInBuffer();
                    _serialPort.DiscardOutBuffer();

                    result = _serialPort.IsOpen;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error openning COM port - '" + ex.Message + "'.");
            }
            return result;
        }
        public void Close()
        {
             if (_serialPort.IsOpen)
             {
                _serialPort.Close();
             }
        }
        public bool  Connect()
        {
            SendHandshakeCommand();
            byte[] data = new byte[0];
            return ReceiveHandshakeCommandResponse(ref data);
        }
        public bool Connected()
        {
            return _serialPort.IsOpen;
        }
        public string GetVersion()
        {
            string result = "error getting version";
            SendReadBootloaderVersionCommand();
            byte[] data = new byte[0];
            if (ReceiveBootloaderVersionCommandResponse(ref data))
            {
                result = "v" + data[1].ToString() + "." + data[2].ToString() + "." + data[3].ToString();
            }

            return result;
        }
        public bool Erase()
        {
            bool result = false;

            return result;
        }
        public bool StartFrimware()
        {
            SendStartApplicationCommand();
            byte[] data = new byte[0];
            return ReceiveStartApplicationCommandResponse(ref data);
        }
        public bool ResetBoard()
        {
            SendResetBoardCommand();
            byte[] data = new byte[0];
            return ReceiveResetBoardCommandResponse(ref data);
        }
        public void ProgramFlash(byte[] firmwware, ResultUpdate resultUpdate=null, ProgressUpdate progressUpdate= null)
        {
            _resultUpdate = resultUpdate; 
            _progressUpdate = progressUpdate;

            //copy to internal array
            Array.Resize(ref _firmwware, firmwware.Length);
            Array.Copy(firmwware, _firmwware, firmwware.Length);

            //align to page size and fill with 0xFF if needed
            if ((_firmwware.Length % PIC32MZ_FLASH_ERASE_BLOCK_SIZE) != 0)
            {
                long len = _firmwware.Length;
                Array.Resize(ref _firmwware, (_firmwware.Length / PIC32MZ_FLASH_ERASE_BLOCK_SIZE + 1) * PIC32MZ_FLASH_ERASE_BLOCK_SIZE);

                for (long i = len; i < _firmwware.Length; i++)
                {
                    _firmwware[i] = 0xFF;
                }
            }

            Thread programFlashThread = new Thread(new ThreadStart(ProgramFlash));
            programFlashThread.Start();
        }

        public bool Verify(byte[] firmwareBinFile)
        {
            UInt32 firmwareCrc = CalculateCrc32(firmwareBinFile, 0, (UInt32)firmwareBinFile.Length);
            SendVerifyCommand(PIC32MZ_FLASH_START_ADDRESS, (UInt32)firmwareBinFile.Length, firmwareCrc);

            byte[] data = new byte[0];
            return ReceiveVerifyCommandResponse(ref data);
        }


        public delegate void ProgressUpdate(int minimum, int maximum, int value);
        public delegate void ResultUpdate(bool success);

        public Bootloader()
        {
            _firmwware = new byte[0];
            _serialPortName = "";
            _serialPortBaudRate = 115200;
            _serialPort = new SerialPort();
        }
    }
}
