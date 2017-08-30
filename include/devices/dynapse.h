/**
 * @file dynapse.h
 *
 * Dynap-se specific configuration defines and information structures.
 */

#ifndef LIBCAER_DEVICES_DYNAPSE_H_
#define LIBCAER_DEVICES_DYNAPSE_H_

#include "usb.h"
#include "../events/spike.h"
#include "../events/special.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Device type definition for iniLabs Dynap-se FX2-based boards.
 */
#define CAER_DEVICE_DYNAPSE 3

/**
 * Dynap-se chip identifier.
 */
#define DYNAPSE_CHIP_DYNAPSE 64

/**
 * Module address: device-side Multiplexer configuration.
 * The Multiplexer is responsible for mixing, timestamping and outputting
 * (via USB) the various event types generated by the device. It is also
 * responsible for timestamp generation.
 */
#define DYNAPSE_CONFIG_MUX      0
/**
 * Module address: device-side AER configuration (from chip).
 * The AER state machine handshakes with the chip's AER bus and gets the
 * spike events from it. It supports various configurable delays.
 */
#define DYNAPSE_CONFIG_AER      1
/**
 * Module address: device-side chip control configuration.
 * This state machine is responsible for configuring the chip's internal
 * control registers, to set special options and biases.
 */
#define DYNAPSE_CONFIG_CHIP     5
/**
 * Module address: device-side system information.
 * The system information module provides various details on the device, such
 * as currently installed logic revision or clock speeds.
 * All its parameters are read-only.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_dynapse_info'
 * documentation for more details on what information is available.
 */
#define DYNAPSE_CONFIG_SYSINFO  6
/**
 * Module address: device-side USB output configuration.
 * The USB output module forwards the data from the device and the FPGA/CPLD to
 * the USB chip, usually a Cypress FX2 or FX3.
 */
#define DYNAPSE_CONFIG_USB      9
/**
 * Clear CAM content, on all cores of a chip.
 * No arguments are used.
 * Remember to select the chip you want to configure before this!
 */
#define DYNAPSE_CONFIG_CLEAR_CAM 10
/**
 * Clear SRAM content, use one SRAM cell (cell 0, out of the four available)
 * to monitor neurons via USB.
 * 'modAddr' is the chip ID on which to operate, other arguments are unused.
 * Remember to also select the chip you want to configure before this!
 */
#define DYNAPSE_CONFIG_DEFAULT_SRAM 11
/**
 * Setup analog neuron monitoring via SMA connectors.
 * 'modAddr' takes the core ID to be monitored, 'paramAddr' the neuron ID.
 * Remember to select the chip you want to configure before this!
 */
#define DYNAPSE_CONFIG_MONITOR_NEU 12
/**
 * Clear SRAM content, route nothing outside (all four SRAM cells zero).
 * No arguments are used.
 * Remember to select the chip you want to configure before this!
 */
#define DYNAPSE_CONFIG_DEFAULT_SRAM_EMPTY 13

/**
 * Module address: device side SRAM controller configuration.
 * The module holds an address, a word to be written to SRAM,
 * the most recent word read using a read command, and a read/write command.
 * Reads/writes are triggered when the address field is changed.
 * Example: caerDynapseWriteSramWords(devHandle, SRAMData, baseAddr, numWords);
 * Writes numWords words from array SRAMData to the SRAM, starting at baseAddr.
 * This define is for internal use of caerDynapseWriteSramWords(); it can be
 * used on its own, but we recommend using the above function that hides all
 * the internal details of writing to the FPGA SRAM.
 */
#define DYNAPSE_CONFIG_SRAM 14

/**
 * Module address: Device side Synapse Reconfiguration module configuration.
 * Provides run control, selection between using a single kernel for
 * all neurons and reading per-neuron kernels from SRAM, programming of the
 * global kernel, as well as target output chip ID selection and SRAM kernel
 * table base address.
 */
#define DYNAPSE_CONFIG_SYNAPSERECONFIG 15

/**
 * Module address: Device side spike generator module configuration.
 * Provides start/stop control of spike train application and selection
 * of fixed/variable inter-spike intervals and their location in memory.
 */
#define DYNAPSE_CONFIG_SPIKEGEN 16

/**
 * Module address: Device side poisson generator configuration
 * Provides run/stop control of poisson spike generation and
 * rate setting for 1024 sources.
 */
#define DYNAPSE_CONFIG_POISSONSPIKEGEN 18

/**
 * Parameter address for module DYNAPSE_CONFIG_POISSONSPIKEGEN:
 * Enables or disables generation of poisson spike trains.
 */
#define DYNAPSE_CONFIG_POISSONSPIKEGEN_RUN 0

/**
 * Parameter address for module DYNAPSE_CONFIG_POISSONSPIKEGEN:
 * Selects the address of a poisson spike train source. Writing
 * to this parameter will apply the rate previously written to the
 * WRITEDATA field.
 */
#define DYNAPSE_CONFIG_POISSONSPIKEGEN_WRITEADDRESS 1

/**
 * Parameter address for module DYNAPSE_CONFIG_POISSONSPIKEGEN:
 * Holds data that will be written to the address specified by
 * WRITEADDRESS.
 */
#define DYNAPSE_CONFIG_POISSONSPIKEGEN_WRITEDATA 2

/**
 * Parameter address for module DYNAPSE_CONFIG_POISSONSPIKEGEN:
 * Chip ID of the chip that will receive events generated by the
 * poisson spike generator.
 */
#define DYNAPSE_CONFIG_POISSONSPIKEGEN_CHIPID 3

/**
 * Parameter address for module DYNAPSE_CONFIG_SPIKEGEN:
 * Instructs the spike generator to start applying the configured
 * spike train when the parameter changes from false to true.
 */
#define DYNAPSE_CONFIG_SPIKEGEN_RUN 0

/**
 * Parameter address for module DYNAPSE_CONFIG_SPIKEGEN:
 * Selects variable inter-spike interval mode (true) or fixed inter-spike
 * interval mode (false).
 */
#define DYNAPSE_CONFIG_SPIKEGEN_VARMODE 1

/**
 * Parameter address for module DYNAPSE_CONFIG_SPIKEGEN:
 * Sets the start address of a spike train in memory.
 */
#define DYNAPSE_CONFIG_SPIKEGEN_BASEADDR 2

/**
 * Paramter address for module DYNAPSE_CONFIG_SPIKEGEN:
 * Sets the number of events to read from memory for a single application
 * of a spike train.
 */
#define DYNAPSE_CONFIG_SPIKEGEN_STIMCOUNT 3

/**
 * Parameter address for module DYNAPSE_CONFIG_SPIKEGEN:
 * Sets the inter-spike interval that will be used in fixed ISI mode (VARMODE false).
 */
#define DYNAPSE_CONFIG_SPIKEGEN_ISI 4

/**
 * Parameter address for module DYNAPSE_CONFIG_SPIKEGEN:
 * Sets the time base resolution for inter-spike intervals as the number
 * of FPGA clock cycles.
 */
#define DYNAPSE_CONFIG_SPIKEGEN_ISIBASE 5

/**
 * Parameter address for module DYNAPSE_CONFIG_SPIKEGEN:
 * Sets repeat mode to true or false.
 */
#define DYNAPSE_CONFIG_SPIKEGEN_REPEAT 6

/**
 * Parameter address for module DYNAPSE_CONFIG_SYNAPSERECONFIG:
 * Run control. Starts and stops handshaking with DVS.
 */
#define DYNAPSE_CONFIG_SYNAPSERECONFIG_RUN 0

/**
 * Parameter address for module DYNAPSE_CONFIG_SYNAPSERECONFIG:
 * Bits 16 down to 12 select the address in the global kernel table
 * and bits 11 down to 0 specify the data.
 * The 12 data bits are split into 4*3 synaptic weight bits which map
 * onto positive/negative polarity events from 2 DVS pixels.
 */
#define DYNAPSE_CONFIG_SYNAPSERECONFIG_GLOBALKERNEL 1

/**
 * Parameter address for module DYNAPSE_CONFIG_SYNAPSERECONFIG:
 * Boolean parameter for selecting between using kernels stored in
 * SRAM or the global kernel table. 1 for SRAM, 0 for global kernel table.
 */
#define DYNAPSE_CONFIG_SYNAPSERECONFIG_USESRAMKERNELS 2

/**
 * Parameter address for module DYNAPSE_CONFIG_SYNAPSERECONFIG:
 * Select which chip outputs should go to.
 */
#define DYNAPSE_CONFIG_SYNAPSERECONFIG_CHIPSELECT 3

/**
 * Parameter address for module DYNAPSE_CONFIG_SYNAPSERECONFIG:
 * SRAM base address configuration in increments of 32 Kib.
 * Setting this to N will place the SRAM kernel LUT in the range [N*2^15,((N+1)*2^15)-1]
 */
#define DYNAPSE_CONFIG_SYNAPSERECONFIG_SRAMBASEADDR 4

/**
 * Parameter address for module DYNAPSE_CONFIG_SRAM:
 * Holds the address that will be used for the next read/write.
 * Writing or reading this field will trigger the command contained
 * in the command register to be executed on the FPGA.
 */
#define DYNAPSE_CONFIG_SRAM_ADDRESS 1

/**
 * Parameter address for module DYNAPSE_CONFIG_SRAM:
 * Holds the most recently read data from the SRAM.
 * Read-only parameter.
 */
#define DYNAPSE_CONFIG_SRAM_READDATA 2

/**
 * Parameter address for module DYNAPSE_CONFIG_SRAM:
 * Holds the data that will be written on the next write.
 * Example:
 *   caerDeviceConfigSet(devHandle, DYNAPSE_CONFIG_SRAM, DYNAPSE_CONFIG_SRAM_WRITEDATA, wData);
 *   caerDeviceConfigSet(devHandle, DYNAPSE_CONFIG_SRAM, DYNAPSE_CONFIG_SRAM_RWCOMMAND, DYNAPSE_CONFIG_SRAM_WRITE);
 *   caerDeviceConfigSet(devHandle, DYNAPSE_CONFIG_SRAM, DYNAPSE_CONFIG_SRAM_ADDRESS, wAddr);
 * Writes wData to the address specified by wAddr.
 */
#define DYNAPSE_CONFIG_SRAM_WRITEDATA 3

/**
 * Parameter address for module DYNAPSE_CONFIG_SRAM:
 * Holds the command that will be executed when the address field is written to.
 * Example:
 *   caerDeviceConfigSet(devHandle, DYNAPSE_CONFIG_SRAM, DYNAPSE_CONFIG_SRAM_RWCOMMAND, DYNAPSE_CONFIG_SRAM_WRITE);
 * Sets the SRAM controller up for doing writes.
 * DYNAPSE_CONFIG_SRAM_READ and DYNAPSE_CONFIG_SRAM_WRITE are supported.
 */
#define DYNAPSE_CONFIG_SRAM_RWCOMMAND 4

/**
 * Command for module DYNAPSE_CONFIG_SRAM:
 * Read command for the RWCOMMAND field.
 * Example:
 *   caerDeviceConfigSet(devHandle, DYNAPSE_CONFIG_SRAM, DYNAPSE_CONFIG_SRAM_RWCOMMAND, DYNAPSE_CONFIG_SRAM_READ);
 * Sets the SRAM controller up for doing reads.
 */
#define DYNAPSE_CONFIG_SRAM_READ 0

/**
 * Command for module DYNAPSE_CONFIG_SRAM:
 * Write command for the RWCOMMAND field.
 * Example:
 *   caerDeviceConfigSet(devHandle, DYNAPSE_CONFIG_SRAM, DYNAPSE_CONFIG_SRAM_RWCOMMAND, DYNAPSE_CONFIG_SRAM_WRITE);
 * Sets the SRAM controller up for doing writes.
 */
#define DYNAPSE_CONFIG_SRAM_WRITE 1

/**
 * Parameter address for module DYNAPSE_CONFIG_SRAM:
 * Burst mode enable for fast writing. Disables updates on address change and instead updates on data change,
 * while automatically incrementing the writing address. Two 16-bit words are written per 32-bit word sent
 * to the SPI controller starting with the least significant half word.
 */
#define DYNAPSE_CONFIG_SRAM_BURSTMODE 5

/**
 * Parameter address for module DYNAPSE_CONFIG_MUX:
 * run the Multiplexer state machine, which is responsible for
 * mixing the various event types at the device level, timestamping
 * them and outputting them via USB or other connectors.
 */
#define DYNAPSE_CONFIG_MUX_RUN                             0
/**
 * Parameter address for module DYNAPSE_CONFIG_MUX:
 * run the Timestamp Generator inside the Multiplexer state machine,
 * which will provide microsecond accurate timestamps to the
 * events passing through.
 */
#define DYNAPSE_CONFIG_MUX_TIMESTAMP_RUN                   1
/**
 * Parameter address for module DYNAPSE_CONFIG_MUX:
 * reset the Timestamp Generator to zero. This also sends a reset
 * pulse to all connected slave devices, resetting their timestamp too.
 */
#define DYNAPSE_CONFIG_MUX_TIMESTAMP_RESET                 2
/**
 * Parameter address for module DYNAPSE_CONFIG_MUX:
 * under normal circumstances, the chip's bias generator is only powered
 * up when either the AER or the configuration state machines are running, to save
 * power. This flag forces the bias generator to be powered up all the time.
 */
#define DYNAPSE_CONFIG_MUX_FORCE_CHIP_BIAS_ENABLE          3
/**
 * Parameter address for module DYNAPSE_CONFIG_MUX:
 * drop AER events if the USB output FIFO is full, instead of having
 * them pile up at the input FIFOs.
 */
#define DYNAPSE_CONFIG_MUX_DROP_AER_ON_TRANSFER_STALL      4

/**
 * Parameter address for module DYNAPSE_CONFIG_AER:
 * run the AER state machine and get spike events from the chip by
 * handshaking with its AER bus.
 */
#define DYNAPSE_CONFIG_AER_RUN                    3
/**
 * Parameter address for module DYNAPSE_CONFIG_AER:
 * delay capturing the data and acknowledging it on the AER bus for
 * the events by this many LogicClock cycles.
 */
#define DYNAPSE_CONFIG_AER_ACK_DELAY          4
/**
 * Parameter address for module DYNAPSE_CONFIG_AER:
 * extend the length of the acknowledge on the AER bus for
 * the events by this many LogicClock cycles.
 */
#define DYNAPSE_CONFIG_AER_ACK_EXTENSION      6
/**
 * Parameter address for module DYNAPSE_CONFIG_AER:
 * if the output FIFO for this module is full, stall the AER handshake with
 * the chip and wait until it's free again, instead of just continuing
 * the handshake and dropping the resulting events.
 */
#define DYNAPSE_CONFIG_AER_WAIT_ON_TRANSFER_STALL 8
/**
 * Parameter address for module DYNAPSE_CONFIG_AER:
 * enable external AER control. This ensures the chip and the neuron
 * array are running, but doesn't do the handshake and leaves the ACK
 * pin in high-impedance, to allow for an external system to take
 * over the AER communication with the chip.
 * DYNAPSE_CONFIG_AER_RUN has to be turned off for this to work.
 */
#define DYNAPSE_CONFIG_AER_EXTERNAL_AER_CONTROL   10

/**
 * Parameter address for module DYNAPSE_CONFIG_CHIP:
 * enable the configuration AER state machine to send
 * bias and control configuration to the chip.
 */
#define DYNAPSE_CONFIG_CHIP_RUN             0
/**
 * Parameter address for module DYNAPSE_CONFIG_CHIP:
 * set the chip ID to which configuration content is
 * being sent.
 */
#define DYNAPSE_CONFIG_CHIP_ID              1
/**
 * Parameter address for module DYNAPSE_CONFIG_CHIP:
 * set the configuration content to send to the chip.
 * Every time this changes, the chip ID is appended
 * and the configuration is sent out to the chip.
 */
#define DYNAPSE_CONFIG_CHIP_CONTENT         2
/**
 * Parameter address for module DYNAPSE_CONFIG_CHIP:
 * delay doing the request after putting out the data
 * by this many LogicClock cycles.
 */
#define DYNAPSE_CONFIG_CHIP_REQ_DELAY       3
/**
 * Parameter address for module DYNAPSE_CONFIG_CHIP:
 * extend the request after receiving the ACK by
 * this many LogicClock cycles.
 */
#define DYNAPSE_CONFIG_CHIP_REQ_EXTENSION   4

/**
 * Parameter address for module DYNAPSE_CONFIG_SYSINFO:
 * read-only parameter, the version of the logic currently
 * running on the device's FPGA/CPLD.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_dynapse_info'
 * documentation to get this information.
 */
#define DYNAPSE_CONFIG_SYSINFO_LOGIC_VERSION    0
/**
 * Parameter address for module DYNAPSE_CONFIG_SYSINFO:
 * read-only parameter, an integer used to identify the different
 * types of sensor chips used on the device.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_dynapse_info'
 * documentation to get this information.
 */
#define DYNAPSE_CONFIG_SYSINFO_CHIP_IDENTIFIER  1
/**
 * Parameter address for module DYNAPSE_CONFIG_SYSINFO:
 * read-only parameter, whether the device is currently a timestamp
 * master or slave when synchronizing multiple devices together.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_dynapse_info'
 * documentation to get this information.
 */
#define DYNAPSE_CONFIG_SYSINFO_DEVICE_IS_MASTER 2
/**
 * Parameter address for module DYNAPSE_CONFIG_SYSINFO:
 * read-only parameter, the frequency in MHz at which the main
 * FPGA/CPLD logic is running.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_dynapse_info'
 * documentation to get this information.
 */
#define DYNAPSE_CONFIG_SYSINFO_LOGIC_CLOCK      3

/**
 * Parameter address for module DYNAPSE_CONFIG_USB:
 * enable the USB FIFO module, which transfers the data from the
 * FPGA/CPLD to the USB chip, to be then sent to the host.
 * Turning this off will suppress any USB data communication!
 */
#define DYNAPSE_CONFIG_USB_RUN                0
/**
 * Parameter address for module DYNAPSE_CONFIG_USB:
 * the time delay after which a packet of data is committed to
 * USB, even if it is not full yet (short USB packet).
 * The value is in 125µs time-slices, corresponding to how
 * USB schedules its operations (a value of 4 for example
 * would mean waiting at most 0.5ms until sending a short
 * USB packet to the host).
 */
#define DYNAPSE_CONFIG_USB_EARLY_PACKET_DELAY 1

/**
 * On-chip SRAM for spike routing.
 */
#define DYNAPSE_CONFIG_SRAM_DIRECTION_POS 0
#define DYNAPSE_CONFIG_SRAM_DIRECTION_NEG 1

#define DYNAPSE_CONFIG_SRAM_DIRECTION_Y_NORTH 0
#define DYNAPSE_CONFIG_SRAM_DIRECTION_Y_SOUTH 1
#define DYNAPSE_CONFIG_SRAM_DIRECTION_X_EAST 0
#define DYNAPSE_CONFIG_SRAM_DIRECTION_X_WEST 1

/**
 * Number of neurons in the x direction of the board.
 */
#define DYNAPSE_X4BOARD_NEUX 64
/**
 * Number of neurons in the y direction of the board.
 */
#define DYNAPSE_X4BOARD_NEUY 64
/**
 * Number of cores in the x direction of the board.
 */
#define DYNAPSE_X4BOARD_COREX 4
/**
 * Number of cores in the y direction of the board.
 */
#define DYNAPSE_X4BOARD_COREY 4

/// Chip 0 ID.
#define DYNAPSE_CONFIG_DYNAPSE_U0	0
/// Chip 1 ID.
#define DYNAPSE_CONFIG_DYNAPSE_U1	8
/// Chip 2 ID.
#define DYNAPSE_CONFIG_DYNAPSE_U2	4
/// Chip 3 ID.
#define DYNAPSE_CONFIG_DYNAPSE_U3	12

/// Number of cores per chip.
#define DYNAPSE_CONFIG_NUMCORES				4
/// Number of neurons in single chip.
#define DYNAPSE_CONFIG_NUMNEURONS			1024
/// Number of neurons per core.
#define DYNAPSE_CONFIG_NUMNEURONS_CORE	 	256
/// Number of columns of neurons in a chip.
#define DYNAPSE_CONFIG_XCHIPSIZE   			32
/// Number of rows of neurons in a core.
#define DYNAPSE_CONFIG_YCHIPSIZE   			32
/// Number of columns of neurons in a core.
#define DYNAPSE_CONFIG_NEUCOL				16
/// Number of rows of neurons in a core.
#define DYNAPSE_CONFIG_NEUROW				16
/// Number of columns of CAMs in a core.
#define DYNAPSE_CONFIG_CAMCOL				16
/// Number of rows of CAMs in a core.
#define DYNAPSE_CONFIG_CAMROW				16
/// Number of CAMs per neuron.
#define DYNAPSE_CONFIG_NUMCAM				64
/// Number of SRAM cells per neuron.
#define DYNAPSE_CONFIG_NUMSRAM_NEU	    	4

/// Fast excitatory synapse.
#define DYNAPSE_CONFIG_CAMTYPE_F_EXC		3
/// Slow excitatory synapse.
#define DYNAPSE_CONFIG_CAMTYPE_S_EXC		2
/// Fast inhibitory synapse.
#define DYNAPSE_CONFIG_CAMTYPE_F_INH		1
/// Slow inhibitory synapse.
#define DYNAPSE_CONFIG_CAMTYPE_S_INH		0

/**
 * Parameter address for module DYNAPSE_CONFIG_BIAS:
 * DYNAPSE chip biases.
 * Bias configuration values must be generated using the proper
 * functions, which are:
 * - caerBiasDynapseGenerate() for Dynap-se coarse-fine (current) biases.
 * See 'https://inilabs.com/support/hardware/user-guide-dynap-se/'
 * section 'Neuron’s behaviors and parameters tuning'.
 */
#define DYNAPSE_CONFIG_BIAS_C0_PULSE_PWLK_P             	0
#define DYNAPSE_CONFIG_BIAS_C0_PS_WEIGHT_INH_S_N            2
#define DYNAPSE_CONFIG_BIAS_C0_PS_WEIGHT_INH_F_N            4
#define DYNAPSE_CONFIG_BIAS_C0_PS_WEIGHT_EXC_S_N         	6
#define DYNAPSE_CONFIG_BIAS_C0_PS_WEIGHT_EXC_F_N        	8
#define DYNAPSE_CONFIG_BIAS_C0_IF_RFR_N          			10
#define DYNAPSE_CONFIG_BIAS_C0_IF_TAU1_N         			12
#define DYNAPSE_CONFIG_BIAS_C0_IF_AHTAU_N           		14
#define DYNAPSE_CONFIG_BIAS_C0_IF_CASC_N 					16
#define DYNAPSE_CONFIG_BIAS_C0_IF_TAU2_N         			18
#define DYNAPSE_CONFIG_BIAS_C0_IF_BUF_P               		20
#define DYNAPSE_CONFIG_BIAS_C0_IF_AHTHR_N             		22
#define DYNAPSE_CONFIG_BIAS_C0_IF_THR_N            			24
#define DYNAPSE_CONFIG_BIAS_C0_NPDPIE_THR_S_P             	26
#define DYNAPSE_CONFIG_BIAS_C0_NPDPIE_THR_F_P            	28
#define DYNAPSE_CONFIG_BIAS_C0_NPDPII_THR_F_P      			30
#define DYNAPSE_CONFIG_BIAS_C0_NPDPII_THR_S_P            	32
#define DYNAPSE_CONFIG_BIAS_C0_IF_NMDA_N            		34
#define DYNAPSE_CONFIG_BIAS_C0_IF_DC_P           			36
#define DYNAPSE_CONFIG_BIAS_C0_IF_AHW_P          			38
#define DYNAPSE_CONFIG_BIAS_C0_NPDPII_TAU_S_P          		40
#define DYNAPSE_CONFIG_BIAS_C0_NPDPII_TAU_F_P 				42
#define DYNAPSE_CONFIG_BIAS_C0_NPDPIE_TAU_F_P         		44
#define DYNAPSE_CONFIG_BIAS_C0_NPDPIE_TAU_S_P               46
#define DYNAPSE_CONFIG_BIAS_C0_R2R_P               			48

#define DYNAPSE_CONFIG_BIAS_C1_PULSE_PWLK_P             	1
#define DYNAPSE_CONFIG_BIAS_C1_PS_WEIGHT_INH_S_N            3
#define DYNAPSE_CONFIG_BIAS_C1_PS_WEIGHT_INH_F_N            5
#define DYNAPSE_CONFIG_BIAS_C1_PS_WEIGHT_EXC_S_N         	7
#define DYNAPSE_CONFIG_BIAS_C1_PS_WEIGHT_EXC_F_N        	9
#define DYNAPSE_CONFIG_BIAS_C1_IF_RFR_N          			11
#define DYNAPSE_CONFIG_BIAS_C1_IF_TAU1_N         			13
#define DYNAPSE_CONFIG_BIAS_C1_IF_AHTAU_N           		15
#define DYNAPSE_CONFIG_BIAS_C1_IF_CASC_N 					17
#define DYNAPSE_CONFIG_BIAS_C1_IF_TAU2_N         			19
#define DYNAPSE_CONFIG_BIAS_C1_IF_BUF_P               		21
#define DYNAPSE_CONFIG_BIAS_C1_IF_AHTHR_N             		23
#define DYNAPSE_CONFIG_BIAS_C1_IF_THR_N            			25
#define DYNAPSE_CONFIG_BIAS_C1_NPDPIE_THR_S_P             	27
#define DYNAPSE_CONFIG_BIAS_C1_NPDPIE_THR_F_P            	29
#define DYNAPSE_CONFIG_BIAS_C1_NPDPII_THR_F_P      			31
#define DYNAPSE_CONFIG_BIAS_C1_NPDPII_THR_S_P            	33
#define DYNAPSE_CONFIG_BIAS_C1_IF_NMDA_N            		35
#define DYNAPSE_CONFIG_BIAS_C1_IF_DC_P           			37
#define DYNAPSE_CONFIG_BIAS_C1_IF_AHW_P          			39
#define DYNAPSE_CONFIG_BIAS_C1_NPDPII_TAU_S_P          		41
#define DYNAPSE_CONFIG_BIAS_C1_NPDPII_TAU_F_P 				43
#define DYNAPSE_CONFIG_BIAS_C1_NPDPIE_TAU_F_P         		45
#define DYNAPSE_CONFIG_BIAS_C1_NPDPIE_TAU_S_P               47
#define DYNAPSE_CONFIG_BIAS_C1_R2R_P               			49

#define DYNAPSE_CONFIG_BIAS_U_BUFFER         				50
#define DYNAPSE_CONFIG_BIAS_U_SSP               			51
#define DYNAPSE_CONFIG_BIAS_U_SSN               			52

#define DYNAPSE_CONFIG_BIAS_C2_PULSE_PWLK_P             	64
#define DYNAPSE_CONFIG_BIAS_C2_PS_WEIGHT_INH_S_N            66
#define DYNAPSE_CONFIG_BIAS_C2_PS_WEIGHT_INH_F_N            68
#define DYNAPSE_CONFIG_BIAS_C2_PS_WEIGHT_EXC_S_N         	70
#define DYNAPSE_CONFIG_BIAS_C2_PS_WEIGHT_EXC_F_N        	72
#define DYNAPSE_CONFIG_BIAS_C2_IF_RFR_N          			74
#define DYNAPSE_CONFIG_BIAS_C2_IF_TAU1_N         			76
#define DYNAPSE_CONFIG_BIAS_C2_IF_AHTAU_N           		78
#define DYNAPSE_CONFIG_BIAS_C2_IF_CASC_N 					80
#define DYNAPSE_CONFIG_BIAS_C2_IF_TAU2_N         			82
#define DYNAPSE_CONFIG_BIAS_C2_IF_BUF_P               		84
#define DYNAPSE_CONFIG_BIAS_C2_IF_AHTHR_N             		86
#define DYNAPSE_CONFIG_BIAS_C2_IF_THR_N            			88
#define DYNAPSE_CONFIG_BIAS_C2_NPDPIE_THR_S_P             	90
#define DYNAPSE_CONFIG_BIAS_C2_NPDPIE_THR_F_P            	92
#define DYNAPSE_CONFIG_BIAS_C2_NPDPII_THR_F_P      			94
#define DYNAPSE_CONFIG_BIAS_C2_NPDPII_THR_S_P            	96
#define DYNAPSE_CONFIG_BIAS_C2_IF_NMDA_N            		98
#define DYNAPSE_CONFIG_BIAS_C2_IF_DC_P           			100
#define DYNAPSE_CONFIG_BIAS_C2_IF_AHW_P          			102
#define DYNAPSE_CONFIG_BIAS_C2_NPDPII_TAU_S_P          		104
#define DYNAPSE_CONFIG_BIAS_C2_NPDPII_TAU_F_P 				106
#define DYNAPSE_CONFIG_BIAS_C2_NPDPIE_TAU_F_P         		108
#define DYNAPSE_CONFIG_BIAS_C2_NPDPIE_TAU_S_P               110
#define DYNAPSE_CONFIG_BIAS_C2_R2R_P               			112

#define DYNAPSE_CONFIG_BIAS_C3_PULSE_PWLK_P             	65
#define DYNAPSE_CONFIG_BIAS_C3_PS_WEIGHT_INH_S_N            67
#define DYNAPSE_CONFIG_BIAS_C3_PS_WEIGHT_INH_F_N            69
#define DYNAPSE_CONFIG_BIAS_C3_PS_WEIGHT_EXC_S_N         	71
#define DYNAPSE_CONFIG_BIAS_C3_PS_WEIGHT_EXC_F_N        	73
#define DYNAPSE_CONFIG_BIAS_C3_IF_RFR_N          			75
#define DYNAPSE_CONFIG_BIAS_C3_IF_TAU1_N         			77
#define DYNAPSE_CONFIG_BIAS_C3_IF_AHTAU_N           		79
#define DYNAPSE_CONFIG_BIAS_C3_IF_CASC_N 					81
#define DYNAPSE_CONFIG_BIAS_C3_IF_TAU2_N         			83
#define DYNAPSE_CONFIG_BIAS_C3_IF_BUF_P               		85
#define DYNAPSE_CONFIG_BIAS_C3_IF_AHTHR_N             		87
#define DYNAPSE_CONFIG_BIAS_C3_IF_THR_N            			89
#define DYNAPSE_CONFIG_BIAS_C3_NPDPIE_THR_S_P             	91
#define DYNAPSE_CONFIG_BIAS_C3_NPDPIE_THR_F_P            	93
#define DYNAPSE_CONFIG_BIAS_C3_NPDPII_THR_F_P      			95
#define DYNAPSE_CONFIG_BIAS_C3_NPDPII_THR_S_P            	97
#define DYNAPSE_CONFIG_BIAS_C3_IF_NMDA_N            		99
#define DYNAPSE_CONFIG_BIAS_C3_IF_DC_P           			101
#define DYNAPSE_CONFIG_BIAS_C3_IF_AHW_P          			103
#define DYNAPSE_CONFIG_BIAS_C3_NPDPII_TAU_S_P          		105
#define DYNAPSE_CONFIG_BIAS_C3_NPDPII_TAU_F_P 				107
#define DYNAPSE_CONFIG_BIAS_C3_NPDPIE_TAU_F_P         		109
#define DYNAPSE_CONFIG_BIAS_C3_NPDPIE_TAU_S_P               111
#define DYNAPSE_CONFIG_BIAS_C3_R2R_P               			113

#define DYNAPSE_CONFIG_BIAS_D_BUFFER         				114
#define DYNAPSE_CONFIG_BIAS_D_SSP               			115
#define DYNAPSE_CONFIG_BIAS_D_SSN               			116

/**
 * Dynap-se device-related information.
 */
struct caer_dynapse_info {
	/// Unique device identifier. Also 'source' for events.
	int16_t deviceID;
	/// Device serial number.
	char deviceSerialNumber[8 + 1];
	/// Device USB bus number.
	uint8_t deviceUSBBusNumber;
	/// Device USB device address.
	uint8_t deviceUSBDeviceAddress;
	/// Device information string, for logging purposes.
	char *deviceString;
	/// Logic (FPGA/CPLD) version.
	int16_t logicVersion;
	/// Whether the device is a time-stamp master or slave.
	bool deviceIsMaster;
	/// Clock in MHz for main logic (FPGA/CPLD).
	int16_t logicClock;
	/// Chip identifier/type.
	int16_t chipID;
};

/**
 * Return basic information on the device, such as its ID, the logic
 * version, and so on. See the 'struct caer_dynapse_info' documentation
 * for more details.
 *
 * @param handle a valid device handle.
 *
 * @return a copy of the device information structure if successful,
 *         an empty structure (all zeros) on failure.
 */
struct caer_dynapse_info caerDynapseInfoGet(caerDeviceHandle handle);

/**
 * On-chip coarse-fine bias current configuration for Dynap-se.
 * See 'https://inilabs.com/support/hardware/user-guide-dynap-se/'
 * section 'Neuron’s behaviors and parameters tuning'.
 */
struct caer_bias_dynapse {
	/// Address of bias to configure, see DYNAPSE_CONFIG_BIAS_* defines.
	uint8_t biasAddress;
	/// Coarse current, from 0 to 7, creates big variations in output current.
	uint8_t coarseValue;
	/// Fine current, from 0 to 255, creates small variations in output current.
	uint8_t fineValue;
	/// Whether this bias is enabled or not.
	bool enabled;
	/// Bias sex: true for 'N' type, false for 'P' type.
	bool sexN;
	/// Bias type: true for 'Normal', false for 'Cascode'.
	bool typeNormal;
	/// Bias current level: true for 'HighBias', false for 'LowBias'.
	bool biasHigh;
};

/**
 * Transform coarse-fine bias structure into internal integer representation,
 * suited for sending directly to the device via caerDeviceConfigSet().
 *
 * @param dynapseBias coarse-fine bias structure.
 *
 * @return internal integer representation for device configuration.
 */
uint32_t caerBiasDynapseGenerate(const struct caer_bias_dynapse dynapseBias);
/**
 * Transform internal integer representation, as received by calls to
 * caerDeviceConfigGet(), into a coarse-fine bias structure, for easier
 * handling and understanding of the various parameters.
 *
 * @param dynapseBias internal integer representation from device.
 *
 * @return coarse-fine bias structure.
 */
struct caer_bias_dynapse caerBiasDynapseParse(const uint32_t dynapseBias);

/**
 * Transfer 16bit words from memory to device SRAM, with configurable
 * starting address and number of words. This works on the FPGA SRAM!
 *
 * @param handle a valid device handle.
 * @param data array from which to read data to send to SRAM.
 * @param baseAddr SRAM start address where to put the data.
 * @param numWords number of 16bit words to transfer.
 *
 * @return true on success, false otherwise.
 */
bool caerDynapseWriteSramWords(caerDeviceHandle handle, const uint16_t *data, uint32_t baseAddr, size_t numWords);

/**
 * Specifies the poisson spike generator's spike rate.
 *
 * @param handle a valid device handle.
 * @param neuronAddr The target neuron of the poisson spike train, range [0,1023].
 * @param rateHz The rate in Hz of the spike train, this will be quantized to the
 *               nearest supported level, range [0,4300].
 *
 * @return true on success, false otherwise.
 */
bool caerDynapseWritePoissonSpikeRate(caerDeviceHandle handle, uint16_t neuronAddr, float rateHz);

/**
 * THIS FUNCTION IS DEPRECATED. USE caerDynapseWriteSramN() INSTEAD!
 * The new function uses the global neuron ID (range [0,1023]) like all others, instead of
 * the separate core ID/neuron ID syntax. Also the arguments are in the same order as
 * caerDynapseGenerateSramBits(), in particular the 'sramId' comes right after 'neuronId'.
 *
 * Write one of the 4 SRAMs of a single neuron. Writing the SRAM means writing the destination
 * address of where the spikes will be routed to. This works on the on-chip SRAM!
 *
 * Remember to select the chip you want to configure before calling this function!
 *
 * @param handle a valid device handle.
 * @param coreId the chip's core ID, range [0,3].
 * @param neuronAddrCore the neuron's address within this core, range [0,255].
 * @param virtualCoreId fake source core ID, set it to this value instead of the actual source core ID, range [0,3].
 * @param sx X direction, can be one of: [DYNAPSE_CONFIG_SRAM_DIRECTION_X_EAST,DYNAPSE_CONFIG_SRAM_DIRECTION_X_WEST].
 * @param dx X delta, number of chips to jumps before reaching destination, range is [0,3].
 * @param sy Y direction, can be one of: [DYNAPSE_CONFIG_SRAM_DIRECTION_Y_NORTH,DYNAPSE_CONFIG_SRAM_DIRECTION_Y_SOUTH].
 * @param dy Y delta, number of chips to jumps before reaching destination, range is [0,3].
 * @param sramId SRAM address (one of four cells), range [0,3].
 * @param destinationCore spike destination core, uses one-hot coding for the 4 cores:
 *                        [C3,C2,C1,C0] -> [0,0,0,0] (0 decimal) no core, [1,1,1,1] (15 decimal) all cores
 *
 * @return true on success, false otherwise.
 */
bool caerDynapseWriteSram(caerDeviceHandle handle, uint8_t coreId, uint8_t neuronAddrCore, uint8_t virtualCoreId, bool sx,
	uint8_t dx, bool sy, uint8_t dy, uint8_t sramId, uint8_t destinationCore) __attribute__ ((__deprecated__));

/**
 * Write one of the 4 SRAMs of a single neuron. Writing the SRAM means writing the destination
 * address of where the spikes will be routed to. This works on the on-chip SRAM!
 *
 * Remember to select the chip you want to configure before calling this function!
 *
 * @param handle a valid device handle.
 * @param neuronAddr the neuron to program, range [0,1023] (use caerDynapseCoreXYToNeuronId() for a 2D mapping).
 * @param sramId SRAM address (one of four cells), range [0,3].
 * @param virtualCoreId fake source core ID, set it to this value instead of the actual source core ID, range [0,3].
 * @param sx X direction, can be one of: [DYNAPSE_CONFIG_SRAM_DIRECTION_X_EAST,DYNAPSE_CONFIG_SRAM_DIRECTION_X_WEST].
 * @param dx X delta, number of chips to jumps before reaching destination, range is [0,3].
 * @param sy Y direction, can be one of: [DYNAPSE_CONFIG_SRAM_DIRECTION_Y_NORTH,DYNAPSE_CONFIG_SRAM_DIRECTION_Y_SOUTH].
 * @param dy Y delta, number of chips to jumps before reaching destination, range is [0,3].
 * @param destinationCore spike destination core, uses one-hot coding for the 4 cores:
 *                        [C3,C2,C1,C0] -> [0,0,0,0] (0 decimal) no core, [1,1,1,1] (15 decimal) all cores
 *
 * @return true on success, false otherwise.
 */
bool caerDynapseWriteSramN(caerDeviceHandle handle, uint16_t neuronAddr, uint8_t sramId, uint8_t virtualCoreId, bool sx,
	uint8_t dx, bool sy, uint8_t dy, uint8_t destinationCore);

/**
 * Write a single CAM, to specify which spikes are allowed as input into a neuron.
 *
 * Remember to select the chip you want to configure before calling this function!
 *
 * @param handle a valid device handle.
 * @param inputNeuronAddr the neuron address that should be let in as input to this neuron, range [0,1023].
 * @param neuronAddr the neuron address whose CAM should be programmed, range [0,1023].
 * @param camId CAM address (synapse), each neuron has 64, range [0,63].
 * @param synapseType one of the four possible synaptic weights:
 *     [DYNAPSE_CONFIG_CAMTYPE_F_EXC,DYNAPSE_CONFIG_CAMTYPE_S_EXC,DYNAPSE_CONFIG_CAMTYPE_F_INH,DYNAPSE_CONFIG_CAMTYPE_S_INH].
 *
 * @return true on success, false otherwise.
 */
bool caerDynapseWriteCam(caerDeviceHandle handle, uint16_t inputNeuronAddr, uint16_t neuronAddr, uint8_t camId,
	uint8_t synapseType);

/**
 * Send array of configuration parameters to the device via USB.
 *
 * Remember to select the chip you want to configure before calling this function!
 *
 * @param handle a valid device handle.
 * @param data an array of integers holding configuration data.
 * @param numConfig number of configuration parameters to send.
 *
 * @return true on success, false otherwise.
 */
bool caerDynapseSendDataToUSB(caerDeviceHandle handle, const uint32_t *data, size_t numConfig);

/**
 * Generate bits to write a single CAM, to specify which spikes are allowed as input into a neuron.
 *
 * @param inputNeuronAddr the neuron address that should be let in as input to this neuron, range [0,1023]
 *     (use caerDynapseCoreXYToNeuronId() for a 2D mapping).
 * @param neuronAddr the neuron to program, range [0,1023] (use caerDynapseCoreXYToNeuronId() for a 2D mapping).
 * @param camId CAM address (synapse), each neuron has 64, range [0,63].
 * @param synapseType one of the four possible synaptic weights:
 *     [DYNAPSE_CONFIG_CAMTYPE_F_EXC,DYNAPSE_CONFIG_CAMTYPE_S_EXC,DYNAPSE_CONFIG_CAMTYPE_F_INH,DYNAPSE_CONFIG_CAMTYPE_S_INH].
 *
 * @return bits to send to device.
 */
uint32_t caerDynapseGenerateCamBits(uint16_t inputNeuronAddr, uint16_t neuronAddr, uint8_t camId, uint8_t synapseType);

/**
 * Generate bits to write one of the 4 SRAMs of a single neuron.
 * Writing the SRAM means writing the destination address of where
 * the spikes will be routed to. This works on the on-chip SRAM!
 *
 * @param neuronAddr the neuron to program, range [0,1023] (use caerDynapseCoreXYToNeuronId() for a 2D mapping).
 * @param sramId SRAM address (one of four cells), range [0,3].
 * @param virtualCoreId fake source core ID, set it to this value instead of the actual source core ID, range [0,3].
 * @param sx X direction, can be one of: [DYNAPSE_CONFIG_SRAM_DIRECTION_X_EAST,DYNAPSE_CONFIG_SRAM_DIRECTION_X_WEST].
 * @param dx X delta, number of chips to jumps before reaching destination, range is [0,3].
 * @param sy Y direction, can be one of: [DYNAPSE_CONFIG_SRAM_DIRECTION_Y_NORTH,DYNAPSE_CONFIG_SRAM_DIRECTION_Y_SOUTH].
 * @param dy Y delta, number of chips to jumps before reaching destination, range is [0,3].
 * @param destinationCore spike destination core, uses one-hot coding for the 4 cores:
 *                        [C3,C2,C1,C0] -> [0,0,0,0] (0 decimal) no core, [1,1,1,1] (15 decimal) all cores
 *
 * @return bits to send to device.
 */
uint32_t caerDynapseGenerateSramBits(uint16_t neuronAddr, uint8_t sramId, uint8_t virtualCoreId, bool sx, uint8_t dx,
	bool sy, uint8_t dy, uint8_t destinationCore);

/**
 * Map core ID and column/row address to the correct chip global neuron address.
 *
 * @param coreId the chip's core ID, range [0,3].
 * @param columnX the neuron's column address, range [0,15].
 * @param rowY the neuron's row address, range [0,15].
 *
 * @return chip global neuron address.
 */
uint16_t caerDynapseCoreXYToNeuronId(uint8_t coreId, uint8_t columnX, uint8_t rowY);

/**
 * Map core ID and per-core neuron address to the correct chip global neuron address.
 *
 * @param coreId the chip's core ID, range [0,3].
 * @param neuronAddrCore the neuron's address within this core, range [0,255].
 *
 * @return chip global neuron address.
 */
uint16_t caerDynapseCoreAddrToNeuronId(uint8_t coreId, uint8_t neuronAddrCore);

/**
 * Get the X (column) address for a spike event, in pixels.
 * The (0, 0) address is in the upper left corner.
 *
 * @param event a valid SpikeEvent pointer. Cannot be NULL.
 *
 * @return the event X address in pixels.
 */
uint16_t caerDynapseSpikeEventGetX(caerSpikeEventConst event);

/**
 * Get the Y (row) address for a spike event, in pixels.
 * The (0, 0) address is in the upper left corner.
 *
 * @param event a valid SpikeEvent pointer. Cannot be NULL.
 *
 * @return the event Y address in pixels.
 */
uint16_t caerDynapseSpikeEventGetY(caerSpikeEventConst event);

/**
 * Get the chip ID, core ID and neuron ID from the X and Y
 * coordinates. This is the reverse transform to:
 * caerDynapseSpikeEventGetX() / caerDynapseSpikeEventGetY().
 * The return value is a 'struct caer_spike_event' because it
 * already has functions to get/set all the needed values.
 *
 * @param x a X coordinate as returned by caerDynapseSpikeEventGetX().
 * @param y a Y coordinate as returned by caerDynapseSpikeEventGetY().
 *
 * @return a SpikeEvent struct holding chip ID, core ID and neuron ID.
 */
struct caer_spike_event caerDynapseSpikeEventFromXY(uint16_t x, uint16_t y);

#ifdef __cplusplus
}
#endif

#endif /* LIBCAER_DEVICES_DYNAPSE_H_ */
