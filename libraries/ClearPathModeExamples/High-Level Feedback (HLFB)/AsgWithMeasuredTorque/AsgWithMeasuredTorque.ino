/*
 * Title: AsgWithMeasuredTorque
 *
 * Objective:
 *    This example demonstrates how to configure and read the High-Level
 *    Feedback output mode "ASG-Position with Measured Torque" or "ASG-Velocity
 *    with Measured Torque" from a ClearPath motor.
 *
 *    Note: There are two different versions of the ASG with Measured Torque
 *    HLFB mode, one for Position modes and one for Velocity modes. See the
 *    ClearPath MC/SD manual for a full description of all HLFB modes.
 *
 * Description:
 *    This example reads the state of an attached ClearPath motor's HLFB output
 *    in All Systems Go with Measured Torque mode. During operation, the state
 *    of the HLFB is written to the USB serial port.
 *
 *    This example does not enable the motor or command any motion. Use the
 *    Motion Generator in MSP to easily exercise the full features of this
 *    example and see HLFB change state and give a torque measurement.
 *
 * Requirements:
 * 1. A ClearPath motor must be connected to Connector M-0.
 * 2. To command motion, the connected ClearPath motor must be configured for
 *    the Motion Generator mode through the MSP software (In MSP select Mode>>
 *    Motion Generator).
 * 3. The connected ClearPath motor must have its HLFB mode set to ASG with
 *    measured torque through the MSP software (select Advanced>>High Level
 *    Feedback [Mode]... then choose "ASG-Position, w/Measured Torque" or
 *    "ASG-Velocity, w/ Measured Torque" and hit the OK button).
 *    Select a 482 Hz PWM Carrier Frequency in this menu.
 *
 * Links:
 * ** ClearCore Documentation: https://teknic-inc.github.io/ClearCore-library/
 * ** ClearCore Manual: https://www.teknic.com/files/downloads/clearcore_user_manual.pdf
 * ** ClearPath Manual (DC Power): https://www.teknic.com/files/downloads/clearpath_user_manual.pdf
 * ** ClearPath Manual (AC Power): https://www.teknic.com/files/downloads/ac_clearpath-mc-sd_manual.pdf
 *
 *
 * Copyright (c) 2020 Teknic Inc. This work is free to use, copy and distribute under the terms of
 * the standard MIT permissive software license which can be found at https://opensource.org/licenses/MIT
 */

#include "ClearCore.h"

// Specify which motor to move.
// Options are: ConnectorM0, ConnectorM1, ConnectorM2, or ConnectorM3.
#define motor ConnectorM0

// Select the baud rate to match the target serial device
#define baudRate 9600

void setup() {
    // Put your setup code here, it will run once:

    // Put the motor connector into the HLFB mode to read bipolar PWM (the
    // correct mode for ASG w/ Measured Torque)
    motor.HlfbMode(MotorDriver::HLFB_MODE_HAS_BIPOLAR_PWM);
    // Set the HFLB carrier frequency to 482 Hz
    motor.HlfbCarrier(MotorDriver::HLFB_CARRIER_482_HZ);

    // Set up serial communication and wait up to 5 seconds for a port to open
    // HLFB states are written to the serial port
    Serial.begin(baudRate);
    uint32_t timeout = 5000;
    uint32_t startTime = millis();
    while (!Serial && millis() - startTime < timeout) {
        continue;
    }
}

void loop() {
    // Put your main code here, it will run repeatedly:

    Serial.print("HLFB state: ");

    // Check the current state of the ClearPath's HLFB.
    MotorDriver::HlfbStates hlfbState = motor.HlfbState();

    // Write the HLFB state to the serial port
    if (hlfbState == MotorDriver::HLFB_HAS_MEASUREMENT) {
        // Writes the torque measured, as a percent of motor peak torque rating
        Serial.print(int(round(motor.HlfbPercent())));
        Serial.println("% torque");
    }
    else if (hlfbState == MotorDriver::HLFB_ASSERTED) {
        // Asserted indicates either "Move Done" for position modes, or
        // "At Target Velocity" for velocity moves
        Serial.println("ASSERTED");
    }
    else {
        Serial.println("DISABLED or SHUTDOWN");
    }

    // Wait before reading HLFB again
    delay(500);
}
