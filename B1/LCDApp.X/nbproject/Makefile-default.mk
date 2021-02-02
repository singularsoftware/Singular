#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/LCDApp.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/LCDApp.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../common/clock.c ../common/gpio.c ../common/irq.c ../common/uart.c ../common/system.c ../common/cp0.c ../common/spi.c ../ST7735SChipDriver/lcd.c main.c initialize.c ../ST7735SChipDriver/lcdfonts.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1270477542/clock.o ${OBJECTDIR}/_ext/1270477542/gpio.o ${OBJECTDIR}/_ext/1270477542/irq.o ${OBJECTDIR}/_ext/1270477542/uart.o ${OBJECTDIR}/_ext/1270477542/system.o ${OBJECTDIR}/_ext/1270477542/cp0.o ${OBJECTDIR}/_ext/1270477542/spi.o ${OBJECTDIR}/_ext/1910017717/lcd.o ${OBJECTDIR}/main.o ${OBJECTDIR}/initialize.o ${OBJECTDIR}/_ext/1910017717/lcdfonts.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1270477542/clock.o.d ${OBJECTDIR}/_ext/1270477542/gpio.o.d ${OBJECTDIR}/_ext/1270477542/irq.o.d ${OBJECTDIR}/_ext/1270477542/uart.o.d ${OBJECTDIR}/_ext/1270477542/system.o.d ${OBJECTDIR}/_ext/1270477542/cp0.o.d ${OBJECTDIR}/_ext/1270477542/spi.o.d ${OBJECTDIR}/_ext/1910017717/lcd.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/initialize.o.d ${OBJECTDIR}/_ext/1910017717/lcdfonts.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1270477542/clock.o ${OBJECTDIR}/_ext/1270477542/gpio.o ${OBJECTDIR}/_ext/1270477542/irq.o ${OBJECTDIR}/_ext/1270477542/uart.o ${OBJECTDIR}/_ext/1270477542/system.o ${OBJECTDIR}/_ext/1270477542/cp0.o ${OBJECTDIR}/_ext/1270477542/spi.o ${OBJECTDIR}/_ext/1910017717/lcd.o ${OBJECTDIR}/main.o ${OBJECTDIR}/initialize.o ${OBJECTDIR}/_ext/1910017717/lcdfonts.o

# Source Files
SOURCEFILES=../common/clock.c ../common/gpio.c ../common/irq.c ../common/uart.c ../common/system.c ../common/cp0.c ../common/spi.c ../ST7735SChipDriver/lcd.c main.c initialize.c ../ST7735SChipDriver/lcdfonts.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/LCDApp.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ2048EFH144
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1270477542/clock.o: ../common/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/clock.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/clock.o.d" -o ${OBJECTDIR}/_ext/1270477542/clock.o ../common/clock.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1270477542/gpio.o: ../common/gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/gpio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/gpio.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/gpio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/gpio.o.d" -o ${OBJECTDIR}/_ext/1270477542/gpio.o ../common/gpio.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1270477542/irq.o: ../common/irq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/irq.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/irq.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/irq.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/irq.o.d" -o ${OBJECTDIR}/_ext/1270477542/irq.o ../common/irq.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1270477542/uart.o: ../common/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/uart.o.d" -o ${OBJECTDIR}/_ext/1270477542/uart.o ../common/uart.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1270477542/system.o: ../common/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/system.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/system.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/system.o.d" -o ${OBJECTDIR}/_ext/1270477542/system.o ../common/system.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1270477542/cp0.o: ../common/cp0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/cp0.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/cp0.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/cp0.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/cp0.o.d" -o ${OBJECTDIR}/_ext/1270477542/cp0.o ../common/cp0.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1270477542/spi.o: ../common/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/spi.o.d" -o ${OBJECTDIR}/_ext/1270477542/spi.o ../common/spi.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1910017717/lcd.o: ../ST7735SChipDriver/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1910017717" 
	@${RM} ${OBJECTDIR}/_ext/1910017717/lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1910017717/lcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1910017717/lcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1910017717/lcd.o.d" -o ${OBJECTDIR}/_ext/1910017717/lcd.o ../ST7735SChipDriver/lcd.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/initialize.o: initialize.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/initialize.o.d 
	@${RM} ${OBJECTDIR}/initialize.o 
	@${FIXDEPS} "${OBJECTDIR}/initialize.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/initialize.o.d" -o ${OBJECTDIR}/initialize.o initialize.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1910017717/lcdfonts.o: ../ST7735SChipDriver/lcdfonts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1910017717" 
	@${RM} ${OBJECTDIR}/_ext/1910017717/lcdfonts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1910017717/lcdfonts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1910017717/lcdfonts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1910017717/lcdfonts.o.d" -o ${OBJECTDIR}/_ext/1910017717/lcdfonts.o ../ST7735SChipDriver/lcdfonts.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
else
${OBJECTDIR}/_ext/1270477542/clock.o: ../common/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/clock.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/clock.o.d" -o ${OBJECTDIR}/_ext/1270477542/clock.o ../common/clock.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1270477542/gpio.o: ../common/gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/gpio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/gpio.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/gpio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/gpio.o.d" -o ${OBJECTDIR}/_ext/1270477542/gpio.o ../common/gpio.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1270477542/irq.o: ../common/irq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/irq.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/irq.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/irq.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/irq.o.d" -o ${OBJECTDIR}/_ext/1270477542/irq.o ../common/irq.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1270477542/uart.o: ../common/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/uart.o.d" -o ${OBJECTDIR}/_ext/1270477542/uart.o ../common/uart.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1270477542/system.o: ../common/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/system.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/system.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/system.o.d" -o ${OBJECTDIR}/_ext/1270477542/system.o ../common/system.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1270477542/cp0.o: ../common/cp0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/cp0.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/cp0.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/cp0.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/cp0.o.d" -o ${OBJECTDIR}/_ext/1270477542/cp0.o ../common/cp0.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1270477542/spi.o: ../common/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1270477542" 
	@${RM} ${OBJECTDIR}/_ext/1270477542/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1270477542/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1270477542/spi.o.d" -o ${OBJECTDIR}/_ext/1270477542/spi.o ../common/spi.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1910017717/lcd.o: ../ST7735SChipDriver/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1910017717" 
	@${RM} ${OBJECTDIR}/_ext/1910017717/lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1910017717/lcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1910017717/lcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1910017717/lcd.o.d" -o ${OBJECTDIR}/_ext/1910017717/lcd.o ../ST7735SChipDriver/lcd.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/initialize.o: initialize.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/initialize.o.d 
	@${RM} ${OBJECTDIR}/initialize.o 
	@${FIXDEPS} "${OBJECTDIR}/initialize.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/initialize.o.d" -o ${OBJECTDIR}/initialize.o initialize.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1910017717/lcdfonts.o: ../ST7735SChipDriver/lcdfonts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1910017717" 
	@${RM} ${OBJECTDIR}/_ext/1910017717/lcdfonts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1910017717/lcdfonts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1910017717/lcdfonts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -Werror -MMD -MF "${OBJECTDIR}/_ext/1910017717/lcdfonts.o.d" -o ${OBJECTDIR}/_ext/1910017717/lcdfonts.o ../ST7735SChipDriver/lcdfonts.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/LCDApp.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_ICD4=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/LCDApp.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x37F   -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_ICD4=1,--defsym=_min_heap_size=64960,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp=${DFP_DIR}
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/LCDApp.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/LCDApp.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=64960,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp=${DFP_DIR}
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/LCDApp.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
