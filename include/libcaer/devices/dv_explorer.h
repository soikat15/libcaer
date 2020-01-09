/**
 * @file dv_explorer.h
 *
 * DV_EXPLORER specific configuration defines and information structures.
 */

#ifndef LIBCAER_DEVICES_DV_EXPLORER_H_
#define LIBCAER_DEVICES_DV_EXPLORER_H_

#include "../events/imu6.h"
#include "../events/polarity.h"
#include "../events/special.h"

#include "imu_support.h"
#include "usb.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Device type definition for iniVation DV_EXPLORER.
 */
#define CAER_DEVICE_DV_EXPLORER 8

/**
 * Samsung chip identifier.
 * 640x480, semi-synchronous readout.
 */
#define DV_EXPLORER_CHIP_ID 20

/**
 * Module address: device-side Multiplexer configuration.
 * The Multiplexer is responsible for mixing, timestamping and outputting
 * (via USB) the various event types generated by the device. It is also
 * responsible for timestamp generation and synchronization.
 */
#define DVX_MUX 0
/**
 * Module address: device-side DVS configuration.
 * The DVS state machine interacts with the DVS chip and gets the
 * polarity events from it. It supports various configurable delays, as
 * well as advanced filtering capabilities on the polarity events.
 */
#define DVX_DVS 1
/**
 * Module address: device-side IMU (Inertial Measurement Unit) configuration.
 * The IMU module connects to the external IMU chip and sends data on the
 * device's movement in space. It can configure various options on the external
 * chip, such as accelerometer range or gyroscope refresh rate.
 */
#define DVX_IMU 3
/**
 * Module address: device-side External Input (signal detector/generator) configuration.
 * The External Input module is used to detect external signals on the external input
 * jack and inject an event into the event stream when this happens. It can detect pulses
 * of a specific length or rising and falling edges.
 * On some systems, a signal generator module is also present, which can generate
 * PWM-like pulsed signals with configurable timing.
 */
#define DVX_EXTINPUT 4
/**
 * Module address: device-side system information.
 * The system information module provides various details on the device, such
 * as currently installed logic revision or clock speeds.
 * All its parameters are read-only.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_dvexplorer_info'
 * documentation for more details on what information is available.
 */
#define DVX_SYSINFO 6
/**
 * Module address: device-side USB output configuration.
 * The USB output module forwards the data from the device and the FPGA/CPLD to
 * the USB chip, usually a Cypress FX2 or FX3.
 */
#define DVX_USB 9

/**
 * Parameter address for module DVX_MUX:
 * run the Multiplexer state machine, which is responsible for
 * mixing the various event types at the device level, timestamping
 * them and outputting them via USB or other connectors.
 */
#define DVX_MUX_RUN 0
/**
 * Parameter address for module DVX_MUX:
 * run the Timestamp Generator inside the Multiplexer state machine,
 * which will provide microsecond accurate timestamps to the
 * events passing through.
 */
#define DVX_MUX_TIMESTAMP_RUN 1
/**
 * Parameter address for module DVX_MUX:
 * reset the Timestamp Generator to zero. This also sends a reset
 * pulse to all connected slave devices, resetting their timestamp too.
 */
#define DVX_MUX_TIMESTAMP_RESET 2
/**
 * Parameter address for module DVX_MUX:
 * power up the chip's bias generator, enabling the chip to work.
 */
#define DVX_MUX_RUN_CHIP 3
/**
 * Parameter address for module DVX_MUX:
 * drop External Input events if the USB output FIFO is full, instead of having
 * them pile up at the input FIFOs.
 */
#define DVX_MUX_DROP_EXTINPUT_ON_TRANSFER_STALL 4
/**
 * Parameter address for module DVX_MUX:
 * drop DVS events if the USB output FIFO is full, instead of having
 * them pile up at the input FIFOs.
 */
#define DVX_MUX_DROP_DVS_ON_TRANSFER_STALL 5
/**
 * Parameter address for module DVX_MUX:
 * read-only parameter, information about the presence of the
 * statistics feature.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_davis_info'
 * documentation to get this information.
 */
#define DVX_MUX_HAS_STATISTICS 80
/**
 * Parameter address for module DVX_MUX:
 * read-only parameter, representing the number of dropped
 * External Input events on the device due to full USB buffers.
 * This is a 64bit value, and should always be read using the
 * function: caerDeviceConfigGet64().
 */
#define DVX_MUX_STATISTICS_EXTINPUT_DROPPED 81
/**
 * Parameter address for module DVX_MUX:
 * read-only parameter, representing the number of dropped
 * DVS events on the device due to full USB buffers.
 * This is a 64bit value, and should always be read using the
 * function: caerDeviceConfigGet64().
 */
#define DVX_MUX_STATISTICS_DVS_DROPPED 83

/**
 * Parameter address for module DVX_DVS:
 * read-only parameter, contains the X axis resolution of the
 * DVS events returned by the camera.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_davis_info'
 * documentation to get proper size information that already
 * considers the rotation and orientation settings.
 */
#define DVX_DVS_SIZE_COLUMNS 0
/**
 * Parameter address for module DVX_DVS:
 * read-only parameter, contains the Y axis resolution of the
 * DVS events returned by the camera.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_davis_info'
 * documentation to get proper size information that already
 * considers the rotation and orientation settings.
 */
#define DVX_DVS_SIZE_ROWS 1
/**
 * Parameter address for module DVX_DVS:
 * read-only parameter, contains information on the orientation
 * of the X/Y axes, whether they should be inverted or not on
 * the host when parsing incoming events.
 * Bit 2: dvsInvertXY
 * Bit 1: reserved
 * Bit 0: reserved
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_davis_info'
 * documentation to get proper size information that already
 * considers the rotation and orientation settings.
 */
#define DVX_DVS_ORIENTATION_INFO 2
/**
 * Parameter address for module DVX_DVS:
 * run the DVS state machine and read out polarity events from the chip.
 */
#define DVX_DVS_RUN 3
/**
 * Parameter address for module DVX_DVS:
 * if the output FIFO for this module is full, stall the chip readout
 * and wait until it's free again, instead of just continuing
 * reading and dropping the resulting events.
 */
#define DVX_DVS_WAIT_ON_TRANSFER_STALL 4
/**
 * Parameter address for module DVX_DVS:
 * read-only parameter, information about the presence of the
 * statistics feature.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_davis_info'
 * documentation to get this information.
 */
#define DVX_DVS_HAS_STATISTICS 80
/**
 * Parameter address for module DVX_DVS:
 * read-only parameter, representing the number of column
 * transactions completed successfully on the device.
 * This is a 64bit value, and should always be read using the
 * function: caerDeviceConfigGet64().
 */
#define DVX_DVS_STATISTICS_COLUMN 81
/**
 * Parameter address for module DVX_DVS:
 * read-only parameter, representing the number of SGroup
 * transactions completed successfully on the device.
 * This is a 64bit value, and should always be read using the
 * function: caerDeviceConfigGet64().
 */
#define DVX_DVS_STATISTICS_SGROUP 83
/**
 * Parameter address for module DVX_DVS:
 * read-only parameter, representing the number of MGroup
 * transactions completed successfully on the device.
 * This is a 64bit value, and should always be read using the
 * function: caerDeviceConfigGet64().
 */
#define DVX_DVS_STATISTICS_MGROUP 85
/**
 * Parameter address for module DVX_DVS:
 * read-only parameter, representing the number of column size
 * errors on the device.
 * This is a 64bit value, and should always be read using the
 * function: caerDeviceConfigGet64().
 */
#define DVX_DVS_STATISTICS_ERROR_COLUMN 87
/**
 * Parameter address for module DVX_DVS:
 * read-only parameter, representing the number of group size
 * errors on the device.
 * This is a 64bit value, and should always be read using the
 * function: caerDeviceConfigGet64().
 */
#define DVX_DVS_STATISTICS_ERROR_GROUP 89

/**
 * Parameter address for module DVX_IMU:
 * read-only parameter, contains information on the type of IMU
 * chip being used in this device:
 * 0 - no IMU present
 * 3 - Bosch BMI 160
 * This is reserved for internal use and should not be used by
 * anything other than libcaer.
 */
#define DVX_IMU_TYPE 0
/**
 * Parameter address for module DVX_IMU:
 * read-only parameter, contains information on the orientation
 * of the X/Y/Z axes, whether they should be flipped or not on
 * the host when parsing incoming IMU data samples.
 * Bit 2: imuFlipX
 * Bit 1: imuFlipY
 * Bit 0: imuFlipZ
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Generated IMU events are already
 * properly flipped when returned to the user.
 */
#define DVX_IMU_ORIENTATION_INFO 1
/**
 * Parameter address for module DVX_IMU:
 * enable the IMU's accelerometer. This takes the
 * IMU chip out of sleep.
 */
#define DVX_IMU_RUN_ACCELEROMETER 2
/**
 * Parameter address for module DVX_IMU:
 * enable the IMU's gyroscope. This takes the
 * IMU chip out of sleep.
 */
#define DVX_IMU_RUN_GYROSCOPE 3
/**
 * Parameter address for module DVX_IMU:
 * enable the IMU's temperature sensor. This takes the
 * IMU chip out of sleep.
 */
#define DVX_IMU_RUN_TEMPERATURE 4
/**
 * Parameter address for module DVX_IMU:
 * 8 settings:
 * 0 - 12.5 Hz
 * 1 - 25 Hz
 * 2 - 50 Hz
 * 3 - 100 Hz
 * 4 - 200 Hz
 * 5 - 400 Hz
 * 6 - 800 Hz
 * 7 - 1600 Hz
 */
#define DVX_IMU_ACCEL_DATA_RATE 5
/**
 * Parameter address for module DVX_IMU:
 * 3 settings:
 * 0 - OSR4
 * 1 - OSR2
 * 2 - Normal
 */
#define DVX_IMU_ACCEL_FILTER 6
/**
 * Parameter address for module DVX_IMU:
 * 4 settings:
 * 0 - +- 2g
 * 1 - +- 4g
 * 2 - +- 8g
 * 3 - +- 16g
 */
#define DVX_IMU_ACCEL_RANGE 7
/**
 * Parameter address for module DVX_IMU:
 * 8 settings:
 * 0 - 25 Hz
 * 1 - 50 Hz
 * 2 - 100 Hz
 * 3 - 200 Hz
 * 4 - 400 Hz
 * 5 - 800 Hz
 * 6 - 1600 Hz
 * 7 - 3200 Hz
 */
#define DVX_IMU_GYRO_DATA_RATE 8
/**
 * Parameter address for module DVX_IMU:
 * 3 settings:
 * 0 - OSR4
 * 1 - OSR2
 * 2 - Normal
 */
#define DVX_IMU_GYRO_FILTER 9
/**
 * Parameter address for module DVX_IMU:
 * 5 settings:
 * 0 - +- 2000°/s
 * 1 - +- 1000°/s
 * 2 - +- 500°/s
 * 3 - +- 250°/s
 * 4 - +- 125°/s
 */
#define DVX_IMU_GYRO_RANGE 10

/**
 * Parameter address for module DVX_EXTINPUT:
 * enable the signal detector module. It generates events
 * when it sees certain types of signals, such as edges or
 * pulses of a defined length, on the SIGNAL pin of the
 * INPUT synchronization connector.
 * This can be useful to inject events into the event
 * stream in response to external stimuli or controls,
 * such as turning on a LED lamp.
 */
#define DVX_EXTINPUT_RUN_DETECTOR 0
/**
 * Parameter address for module DVX_EXTINPUT:
 * send a special EXTERNAL_INPUT_RISING_EDGE event when a
 * rising edge is detected (transition from low voltage to high).
 */
#define DVX_EXTINPUT_DETECT_RISING_EDGES 1
/**
 * Parameter address for module DVX_EXTINPUT:
 * send a special EXTERNAL_INPUT_FALLING_EDGE event when a
 * falling edge is detected (transition from high voltage to low).
 */
#define DVX_EXTINPUT_DETECT_FALLING_EDGES 2
/**
 * Parameter address for module DVX_EXTINPUT:
 * send a special EXTERNAL_INPUT_PULSE event when a pulse, of
 * a specified, configurable polarity and length, is detected.
 * See DVX_EXTINPUT_DETECT_PULSE_POLARITY and
 * DVX_EXTINPUT_DETECT_PULSE_LENGTH for more details.
 */
#define DVX_EXTINPUT_DETECT_PULSES 3
/**
 * Parameter address for module DVX_EXTINPUT:
 * the polarity the pulse must exhibit to be detected as such.
 * '1' means active high; a pulse will start when the signal
 * goes from low to high and will continue to be seen as the
 * same pulse as long as it stays high.
 * '0' means active low; a pulse will start when the signal
 * goes from high to low and will continue to be seen as the
 * same pulse as long as it stays low.
 */
#define DVX_EXTINPUT_DETECT_PULSE_POLARITY 4
/**
 * Parameter address for module DVX_EXTINPUT:
 * the minimal length that a pulse must have to trigger the
 * sending of a special event. This is measured in cycles
 * at LogicClock frequency (see 'struct caer_davis_info' for
 * details on how to get the frequency).
 */
#define DVX_EXTINPUT_DETECT_PULSE_LENGTH 5
/**
 * Parameter address for module DVX_EXTINPUT:
 * read-only parameter, information about the presence of the
 * signal generator feature.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_davis_info'
 * documentation to get this information.
 */
#define DVX_EXTINPUT_HAS_GENERATOR 10
/**
 * Parameter address for module DVX_EXTINPUT:
 * enable the signal generator module. It generates a
 * PWM-like signal based on configurable parameters and
 * outputs it on the OUT JACK signal.
 */
#define DVX_EXTINPUT_RUN_GENERATOR 11
/**
 * Parameter address for module DVX_EXTINPUT:
 * polarity of the PWM-like signal to be generated.
 * '1' means active high, '0' means active low.
 */
#define DVX_EXTINPUT_GENERATE_PULSE_POLARITY 12
/**
 * Parameter address for module DVX_EXTINPUT:
 * the interval between the start of two consecutive pulses,
 * expressed in cycles at LogicClock frequency (see
 * 'struct caer_davis_info' for details on how to get the frequency).
 * This must be bigger or equal to DVX_EXTINPUT_GENERATE_PULSE_LENGTH.
 * To generate a signal with 50% duty cycle, this would
 * have to be exactly double of DVX_EXTINPUT_GENERATE_PULSE_LENGTH.
 */
#define DVX_EXTINPUT_GENERATE_PULSE_INTERVAL 13
/**
 * Parameter address for module DVX_EXTINPUT:
 * the length a pulse stays active, expressed in cycles at
 * LogicClock frequency (see 'struct caer_davis_info' for
 * details on how to get the frequency). This must be
 * smaller or equal to DVX_EXTINPUT_GENERATE_PULSE_INTERVAL.
 * To generate a signal with 50% duty cycle, this would
 * have to be exactly half of DVX_EXTINPUT_GENERATE_PULSE_INTERVAL.
 */
#define DVX_EXTINPUT_GENERATE_PULSE_LENGTH 14
/**
 * Parameter address for module DVX_EXTINPUT:
 * enables event injection when a rising edge occurs in the
 * generated signal; a special event EXTERNAL_GENERATOR_RISING_EDGE
 * is emitted into the event stream.
 */
#define DVX_EXTINPUT_GENERATE_INJECT_ON_RISING_EDGE 15
/**
 * Parameter address for module DVX_EXTINPUT:
 * enables event injection when a falling edge occurs in the
 * generated signal; a special event EXTERNAL_GENERATOR_FALLING_EDGE
 * is emitted into the event stream.
 */
#define DVX_EXTINPUT_GENERATE_INJECT_ON_FALLING_EDGE 16

/**
 * Parameter address for module DVX_SYSINFO:
 * read-only parameter, the version of the logic currently
 * running on the device's FPGA/CPLD.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_dvexplorer_info'
 * documentation to get this information.
 */
#define DVX_SYSINFO_LOGIC_VERSION 0
/**
 * Parameter address for module DVX_SYSINFO:
 * read-only parameter, an integer used to identify the different
 * types of sensor chips used on the device.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_dvexplorer_info'
 * documentation to get this information.
 */
#define DVX_SYSINFO_CHIP_IDENTIFIER 1
/**
 * Parameter address for module DVX_SYSINFO:
 * read-only parameter, whether the device is currently a timestamp
 * master or slave when synchronizing multiple devices together.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer. Please see the 'struct caer_dvexplorer_info'
 * documentation to get this information.
 */
#define DVX_SYSINFO_DEVICE_IS_MASTER 2
/**
 * Parameter address for module DVX_SYSINFO:
 * read-only parameter, the frequency in MHz at which the main
 * FPGA/CPLD logic is running.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer.
 */
#define DVX_SYSINFO_LOGIC_CLOCK 3
/**
 * Parameter address for module DVX_SYSINFO:
 * read-only parameter, the frequency in MHz at which the FPGA/CPLD
 * logic related to USB data transmission is running.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer.
 */
#define DVX_SYSINFO_USB_CLOCK 5
/**
 * Parameter address for module DVX_SYSINFO:
 * read-only parameter, the deviation factor for the clocks.
 * Due to how FX3 generates the clocks, which are then used by
 * FPGA/CPLD, they are not integers but have a fractional part.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer.
 */
#define DVX_SYSINFO_CLOCK_DEVIATION 6
/**
 * Parameter address for module DVX_SYSINFO:
 * read-only parameter, the patch version of the logic currently
 * running on the device's FPGA/CPLD.
 * This is reserved for internal use and should not be used by
 * anything other than libcaer.
 */
#define DVX_SYSINFO_LOGIC_PATCH 7

/**
 * Parameter address for module DVX_USB:
 * enable the USB FIFO module, which transfers the data from the
 * FPGA/CPLD to the USB chip, to be then sent to the host.
 * Turning this off will suppress any USB data communication!
 */
#define DVX_USB_RUN 0
/**
 * Parameter address for module DVX_USB:
 * the time delay after which a packet of data is committed to
 * USB, even if it is not full yet (short USB packet).
 * The value is in 125µs time-slices, corresponding to how
 * USB schedules its operations (a value of 4 for example
 * would mean waiting at most 0.5ms until sending a short
 * USB packet to the host).
 */
#define DVX_USB_EARLY_PACKET_DELAY 1

/**
 * Module address: device-side chip configuration.
 * This state machine is responsible for configuring the Samsung DVS chip.
 */
#define DVX_DVS_CHIP 20
#define DVX_DVS_CHIP_MODE 0
#define DVX_DVS_CHIP_EVENT_FLATTEN 1
#define DVX_DVS_CHIP_EVENT_ON_ONLY 2
#define DVX_DVS_CHIP_EVENT_OFF_ONLY 3
#define DVX_DVS_CHIP_SUBSAMPLE_ENABLE 4
#define DVX_DVS_CHIP_AREA_BLOCKING_ENABLE 5
#define DVX_DVS_CHIP_DUAL_BINNING_ENABLE 6
#define DVX_DVS_CHIP_SUBSAMPLE_VERTICAL 7
#define DVX_DVS_CHIP_SUBSAMPLE_HORIZONTAL 8
#define DVX_DVS_CHIP_AREA_BLOCKING_0 9
#define DVX_DVS_CHIP_AREA_BLOCKING_1 10
#define DVX_DVS_CHIP_AREA_BLOCKING_2 11
#define DVX_DVS_CHIP_AREA_BLOCKING_3 12
#define DVX_DVS_CHIP_AREA_BLOCKING_4 13
#define DVX_DVS_CHIP_AREA_BLOCKING_5 14
#define DVX_DVS_CHIP_AREA_BLOCKING_6 15
#define DVX_DVS_CHIP_AREA_BLOCKING_7 16
#define DVX_DVS_CHIP_AREA_BLOCKING_8 17
#define DVX_DVS_CHIP_AREA_BLOCKING_9 18
#define DVX_DVS_CHIP_AREA_BLOCKING_10 19
#define DVX_DVS_CHIP_AREA_BLOCKING_11 20
#define DVX_DVS_CHIP_AREA_BLOCKING_12 21
#define DVX_DVS_CHIP_AREA_BLOCKING_13 22
#define DVX_DVS_CHIP_AREA_BLOCKING_14 23
#define DVX_DVS_CHIP_AREA_BLOCKING_15 24
#define DVX_DVS_CHIP_AREA_BLOCKING_16 25
#define DVX_DVS_CHIP_AREA_BLOCKING_17 26
#define DVX_DVS_CHIP_AREA_BLOCKING_18 27
#define DVX_DVS_CHIP_AREA_BLOCKING_19 28
#define DVX_DVS_CHIP_TIMESTAMP_RESET 29
#define DVX_DVS_CHIP_GLOBAL_RESET_ENABLE 30
#define DVX_DVS_CHIP_GLOBAL_RESET_DURING_READOUT 31
#define DVX_DVS_CHIP_GLOBAL_HOLD_ENABLE 32
#define DVX_DVS_CHIP_FIXED_READ_TIME_ENABLE 33
#define DVX_DVS_CHIP_EXTERNAL_TRIGGER_MODE 34
#define DVX_DVS_CHIP_TIMING_ED 35
#define DVX_DVS_CHIP_TIMING_GH2GRS 36
#define DVX_DVS_CHIP_TIMING_GRS 37
#define DVX_DVS_CHIP_TIMING_GH2SEL 38
#define DVX_DVS_CHIP_TIMING_SELW 39
#define DVX_DVS_CHIP_TIMING_SEL2AY_R 40
#define DVX_DVS_CHIP_TIMING_SEL2AY_F 41
#define DVX_DVS_CHIP_TIMING_SEL2R_R 42
#define DVX_DVS_CHIP_TIMING_SEL2R_F 43
#define DVX_DVS_CHIP_TIMING_NEXT_SEL 44
#define DVX_DVS_CHIP_TIMING_NEXT_GH 45
#define DVX_DVS_CHIP_TIMING_READ_FIXED 46

#define DVX_DVS_CHIP_MODE_OFF 0
#define DVX_DVS_CHIP_MODE_MONITOR 1
#define DVX_DVS_CHIP_MODE_STREAM 2

#define DVX_DVS_CHIP_EXTERNAL_TRIGGER_MODE_TIMESTAMP_RESET 0
#define DVX_DVS_CHIP_EXTERNAL_TRIGGER_MODE_SINGLE_FRAME 1

#define DVX_DVS_CHIP_SUBSAMPLE_VERTICAL_NONE 0
#define DVX_DVS_CHIP_SUBSAMPLE_VERTICAL_HALF 1
#define DVX_DVS_CHIP_SUBSAMPLE_VERTICAL_FOURTH 3
#define DVX_DVS_CHIP_SUBSAMPLE_VERTICAL_EIGHTH 7

#define DVX_DVS_CHIP_SUBSAMPLE_HORIZONTAL_NONE 0
#define DVX_DVS_CHIP_SUBSAMPLE_HORIZONTAL_HALF 1
#define DVX_DVS_CHIP_SUBSAMPLE_HORIZONTAL_FOURTH 3
#define DVX_DVS_CHIP_SUBSAMPLE_HORIZONTAL_EIGHTH 7

#define DVX_DVS_CHIP_CROPPER 21
#define DVX_DVS_CHIP_CROPPER_ENABLE 0
#define DVX_DVS_CHIP_CROPPER_Y_START_ADDRESS 1
#define DVX_DVS_CHIP_CROPPER_Y_END_ADDRESS 2
#define DVX_DVS_CHIP_CROPPER_X_START_ADDRESS 3
#define DVX_DVS_CHIP_CROPPER_X_END_ADDRESS 4

#define DVX_DVS_CHIP_ACTIVITY_DECISION 22
#define DVX_DVS_CHIP_ACTIVITY_DECISION_ENABLE 0
#define DVX_DVS_CHIP_ACTIVITY_DECISION_POS_THRESHOLD 1
#define DVX_DVS_CHIP_ACTIVITY_DECISION_NEG_THRESHOLD 2
#define DVX_DVS_CHIP_ACTIVITY_DECISION_DEC_RATE 3
#define DVX_DVS_CHIP_ACTIVITY_DECISION_DEC_TIME 4
#define DVX_DVS_CHIP_ACTIVITY_DECISION_POS_MAX_COUNT 5

#define DVX_DVS_CHIP_BIAS 23
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_LOG 0
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_SF 1
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_ON 2
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_nRST 3
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_LOGA 4
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_LOGD 5
#define DVX_DVS_CHIP_BIAS_CURRENT_LEVEL_SF 6
#define DVX_DVS_CHIP_BIAS_CURRENT_LEVEL_nOFF 7
#define DVX_DVS_CHIP_BIAS_CURRENT_AMP 8
#define DVX_DVS_CHIP_BIAS_CURRENT_ON 9
#define DVX_DVS_CHIP_BIAS_CURRENT_OFF 10
#define DVX_DVS_CHIP_BIAS_SIMPLE 20

#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_LOG_5uA 0
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_LOG_50uA 1
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_SF_0_5uA 0
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_SF_5uA 1
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_ON_5uA 0
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_ON_50uA 1
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_nRST_0_5uA 0
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_nRST_5uA 1
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_LOGA_5uA 0
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_LOGA_50uA 1
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_LOGD_5uA 0
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_LOGD_50uA 1
#define DVX_DVS_CHIP_BIAS_CURRENT_RANGE_LOGD_500uA 2
#define DVX_DVS_CHIP_BIAS_CURRENT_LEVEL_SF_x0_1 0
#define DVX_DVS_CHIP_BIAS_CURRENT_LEVEL_SF_x1 1
#define DVX_DVS_CHIP_BIAS_CURRENT_LEVEL_nOFF_x0_1 0
#define DVX_DVS_CHIP_BIAS_CURRENT_LEVEL_nOFF_x1 1

#define DVX_DVS_CHIP_BIAS_SIMPLE_VERY_LOW 0
#define DVX_DVS_CHIP_BIAS_SIMPLE_LOW 1
#define DVX_DVS_CHIP_BIAS_SIMPLE_DEFAULT 2
#define DVX_DVS_CHIP_BIAS_SIMPLE_HIGH 3
#define DVX_DVS_CHIP_BIAS_SIMPLE_VERY_HIGH 4

/**
 * DV_EXPLORER device-related information.
 */
struct caer_dvx_info {
	/// Unique device identifier. Also 'source' for events.
	int16_t deviceID;
	/// Device serial number.
	char deviceSerialNumber[8 + 1];
	/// Device USB bus number.
	uint8_t deviceUSBBusNumber;
	/// Device USB device address.
	uint8_t deviceUSBDeviceAddress;
	/// Device information string, for logging purposes.
	/// If not NULL, pointed-to memory is *only* valid while the corresponding
	/// device is open! After calling deviceClose() this is invalid memory!
	char *deviceString;
	/// USB firmware version.
	int16_t firmwareVersion;
	/// Logic (FPGA/CPLD) version.
	int16_t logicVersion;
	/// Chip identifier/type.
	int16_t chipID;
	/// Whether the device is a time-stamp master or slave.
	bool deviceIsMaster;
	/// Feature test: Multiplexer statistics support (event drops).
	bool muxHasStatistics;
	/// DVS X axis resolution.
	int16_t dvsSizeX;
	/// DVS Y axis resolution.
	int16_t dvsSizeY;
	/// Feature test: DVS statistics support.
	bool dvsHasStatistics;
	/// IMU chip type on device.
	enum caer_imu_types imuType;
	/// Feature test: External Input module supports Signal-Generation.
	bool extInputHasGenerator;
};

/**
 * Return basic information on the device, such as its ID, its
 * resolution, the logic version, and so on. See the 'struct
 * caer_dvx_info' documentation for more details.
 *
 * @param handle a valid device handle.
 *
 * @return a copy of the device information structure if successful,
 *         an empty structure (all zeros) on failure.
 */
struct caer_dvx_info caerDVExplorerInfoGet(caerDeviceHandle handle);

#ifdef __cplusplus
}
#endif

#endif /* LIBCAER_DEVICES_DV_EXPLORER_H_ */
