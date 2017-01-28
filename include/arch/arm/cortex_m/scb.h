/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ARM CORTEX-M System Control Block interface
 *
 * Provide an interface to the System Control Block found on ARM Cortex-M
 * processors.
 *
 * The API does not account for all possible usages of the SCB, only the
 * functionalities needed by the kernel. It does not contain NVIC
 * functionalities either: these can be found in nvic.h. MPU functionalities
 * are not implemented.
 *
 * The same effect can be achieved by directly writing in the registers of the
 * SCB, with the layout available from scs.h, using the __scs.scb data
 * structure (or hardcoded values), but the APIs found here are less
 * error-prone, especially for registers with multiple instances to account
 * for 16 exceptions.
 *
 * If access to a missing functionality is needed, directly writing to the
 * registers is the way to implement it.
 */

#ifndef _SCB__H_
#define _SCB__H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _ASMLANGUAGE

#else

#include <kernel.h>
#include <arch/cpu.h>
#include <misc/__assert.h>
#include <arch/arm/cortex_m/scs.h>
#include <misc/util.h>
#include <stdint.h>

#if defined(CONFIG_ARMV6_M)
#elif defined(CONFIG_ARMV7_M)
/**
 *
 * @brief Find out if a hard fault is caused by a bus error on vector read
 *
 * This routine determines if a hard fault is caused by a bus error during
 * a vector table read operation.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbHardFaultIsBusErrOnVectorRead(void)
{
	return __scs.scb.hfsr.bit.vecttbl;
}

/**
 *
 * @brief Clear all hard faults (HFSR register)
 *
 * HFSR register is a 'write-one-to-clear' (W1C) register.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbHardFaultAllFaultsReset(void)
{
	return __scs.scb.hfsr.val = 0xffff;
}

/**
 *
 * @brief Find out if a hard fault is an MPU fault
 *
 * This routine determines if a hard fault is an MPU fault.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbIsMemFault(void)
{
	return !!__scs.scb.cfsr.byte.mmfsr.val;
}

/**
 *
 * @brief Find out if the MMFAR register contains a valid value
 *
 * The MMFAR register contains the faulting address on an MPU fault.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbMemFaultIsMmfarValid(void)
{
	return !!__scs.scb.cfsr.byte.mmfsr.bit.mmarvalid;
}

/**
 *
 * @brief Invalid the value in MMFAR
 *
 * This routine invalidates the MMFAR value. This should be done after
 * processing an MPU fault.
 *
 * @return N/A
 */

static inline void _ScbMemFaultMmfarReset(void)
{
	__scs.scb.cfsr.byte.mmfsr.bit.mmarvalid = 0;
}

/**
 *
 * @brief Clear all MPU faults (MMFSR register)
 *
 * CFSR/MMFSR register is a 'write-one-to-clear' (W1C) register.
 *
 * @return 1 if so, 0 otherwise
 */

static inline void _ScbMemFaultAllFaultsReset(void)
{
	__scs.scb.cfsr.byte.mmfsr.val = 0xfe;
}

/**
 *
 * @brief Find out if an MPU fault is a stacking fault
 *
 * This routine determines if an MPU fault is a stacking fault.
 * This may occur upon exception entry.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbMemFaultIsStacking(void)
{
	return !!__scs.scb.cfsr.byte.mmfsr.bit.mstkerr;
}

/**
 *
 * @brief Find out if an MPU fault is an unstacking fault
 *
 * This routine determines if an MPU fault is an unstacking fault.
 * This may occur upon exception exit.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbMemFaultIsUnstacking(void)
{
	return !!__scs.scb.cfsr.byte.mmfsr.bit.munstkerr;
}

/**
 *
 * @brief Find out if an MPU fault is a data access violation
 *
 * If this routine returns 1, read the MMFAR register via _ScbMemFaultAddrGet()
 * to get the faulting address.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbMemFaultIsDataAccessViolation(void)
{
	return !!__scs.scb.cfsr.byte.mmfsr.bit.daccviol;
}

/**
 *
 * @brief Find out if an MPU fault is an instruction access violation
 *
 * This routine determines if an MPU fault is due to an instruction access
 * violation.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbMemFaultIsInstrAccessViolation(void)
{
	return !!__scs.scb.cfsr.byte.mmfsr.bit.iaccviol;
}

/**
 *
 * @brief Find out the faulting address on an MPU fault
 *
 * @return the faulting address
 */

static inline uint32_t _ScbMemFaultAddrGet(void)
{
	return __scs.scb.mmfar;
}

/**
 *
 * @brief Find out if a hard fault is a bus fault
 *
 * This routine determines if a hard fault is a bus fault.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbIsBusFault(void)
{
	return !!__scs.scb.cfsr.byte.bfsr.val;
}

/**
 *
 * @brief Find out if the BFAR register contains a valid value
 *
 * The BFAR register contains the faulting address on bus fault.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbBusFaultIsBfarValid(void)
{
	return !!__scs.scb.cfsr.byte.bfsr.bit.bfarvalid;
}

/**
 *
 * @brief Invalid the value in BFAR
 *
 * This routine clears/invalidates the Bus Fault Address Register.
 * It should be done after processing a bus fault.
 *
 * @return N/A
 */

static inline void _ScbBusFaultBfarReset(void)
{
	__scs.scb.cfsr.byte.bfsr.bit.bfarvalid = 0;
}

/**
 *
 * @brief Clear all bus faults (BFSR register)
 *
 * CFSR/BFSR register is a 'write-one-to-clear' (W1C) register.
 *
 * @return N/A
 */

static inline void _ScbBusFaultAllFaultsReset(void)
{
	__scs.scb.cfsr.byte.bfsr.val = 0xfe;
}

/**
 *
 * @brief Find out if a bus fault is a stacking fault
 *
 * This routine determines if a bus fault is a stacking fault.
 * This may occurs upon exception entry.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbBusFaultIsStacking(void)
{
	return !!__scs.scb.cfsr.byte.bfsr.bit.stkerr;
}

/**
 *
 * @brief Find out if a bus fault is an unstacking fault
 *
 * This routine determines if a bus fault is an unstacking fault.
 * This may occur upon exception exit.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbBusFaultIsUnstacking(void)
{
	return !!__scs.scb.cfsr.byte.bfsr.bit.unstkerr;
}

/**
 *
 * @brief Find out if a bus fault is an imprecise error
 *
 * This routine determines if a bus fault is an imprecise error.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbBusFaultIsImprecise(void)
{
	return !!__scs.scb.cfsr.byte.bfsr.bit.impreciserr;
}

/**
 *
 * @brief Find out if a bus fault is an precise error
 *
 * Read the BFAR register via _ScbBusFaultAddrGet() if this routine returns 1,
 * as it will contain the faulting address.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbBusFaultIsPrecise(void)
{
	return !!__scs.scb.cfsr.byte.bfsr.bit.preciserr;
}

/**
 *
 * @brief Find out if a bus fault is an instruction bus error
 *
 * This routine determines if a bus fault is an instruction bus error.
 * It is signalled only if the instruction is issued.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbBusFaultIsInstrBusErr(void)
{
	return !!__scs.scb.cfsr.byte.bfsr.bit.ibuserr;
}

/**
 *
 * @brief Get the faulting address on a precise bus fault
 *
 * This routine returns the faulting address for a precise bus fault.
 *
 * @return the faulting address
 */

static inline uint32_t _ScbBusFaultAddrGet(void)
{
	return __scs.scb.bfar;
}

/**
 *
 * @brief Find out if a hard fault is a usage fault
 *
 * This routine determines if a hard fault is a usage fault.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbIsUsageFault(void)
{
	return !!__scs.scb.cfsr.byte.ufsr.val;
}

/**
 *
 * @brief Find out if a usage fault is a 'divide by zero' fault
 *
 * This routine determines if a usage fault is a 'divide by zero' fault.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbUsageFaultIsDivByZero(void)
{
	return !!__scs.scb.cfsr.byte.ufsr.bit.divbyzero;
}

/**
 *
 * @brief Find out if a usage fault is a unaligned access error
 *
 * This routine determines if a usage fault is an unaligned access error.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbUsageFaultIsUnaligned(void)
{
	return !!__scs.scb.cfsr.byte.ufsr.bit.unaligned;
}

/**
 *
 * @brief Find out if a usage fault is a co-processor access error
 *
 * This routine determines if a usage fault is caused by a co-processor access.
 * This happens if the co-processor is either absent or disabled.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbUsageFaultIsNoCp(void)
{
	return !!__scs.scb.cfsr.byte.ufsr.bit.nocp;
}

/**
 *
 * @brief Find out if a usage fault is a invalid PC load error
 *
 * Happens if the the instruction address on an exception return is not
 * halfword-aligned.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbUsageFaultIsInvalidPcLoad(void)
{
	return !!__scs.scb.cfsr.byte.ufsr.bit.invpc;
}

/**
 *
 * @brief Find out if a usage fault is a invalid state error
 *
 * Happens if the the instruction address loaded in the PC via a branch, LDR or
 * POP, or if the instruction address installed in a exception vector, does not
 * have bit 0 set; i.e, is not halfword-aligned.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbUsageFaultIsInvalidState(void)
{
	return !!__scs.scb.cfsr.byte.ufsr.bit.invstate;
}

/**
 *
 * @brief Find out if a usage fault is a undefined instruction error
 *
 * The processor tried to execute an invalid opcode.
 *
 * @return 1 if so, 0 otherwise
 */

static inline int _ScbUsageFaultIsUndefinedInstr(void)
{
	return !!__scs.scb.cfsr.byte.ufsr.bit.undefinstr;
}

/**
 *
 * @brief Clear all usage faults (UFSR register)
 *
 * CFSR/UFSR register is a 'write-one-to-clear' (W1C) register.
 *
 * @return N/A
 */

static inline void _ScbUsageFaultAllFaultsReset(void)
{
	__scs.scb.cfsr.byte.ufsr.val = 0xffff;
}

#else
#error Unknown ARM architecture
#endif /* CONFIG_ARMV6_M */

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* _SCB__H_ */
