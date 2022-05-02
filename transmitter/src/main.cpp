#include <ftdi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "common/common.hpp"
#include "colors.hpp"
/* ht13.cpp
 * compile: gcc -O2 ht13.c -lftdi -lncurses -o ht13
 * this program transmits dmx-style rgb packets
 * modified to adjust to software PWM on the Mrfs
 * broadcasts from PC/Xbee to Mrf/Arduino
 *
 * really going to try constant broadcast this time
 * and... it flickers really badly on holds
 *
 * Benjamin Jeffery
 * University of Idaho
 * 11/11/2015
 * millisec() is copied from unicon/src/common/time.c
 * for testing purposes. Thanks Clint
 *
 * Some minor edits/cleanup by Lucas Jackson
 * University of Idaho
 * 10/27/2020
 * 
 * Some more minor edits by Hayden Carroll
 * University of Idaho
 * 4/26/2022
 */

#define DOT 100000
#define DASH 300000
#define SLOW 500000
#define DORK 150000
#define DROOL 1000000
#define DAB 50000
#define SLP 40000

// static size_t encode(const uint8_t* source, size_t size, uint8_t*
// destination);
static size_t getEncodedBufferSize(size_t sourceSize);
uint8_t* getEncodedArrayFromImage(const transmitter::Image& image);
bool initializeFTDI(ftdi_context* ftdi);
int setupFTDIConnection(ftdi_context* ftdi);
int closeFTDIConnection(ftdi_context* ftdi);

int main() {
    int x = 32; // num of people in X axis
    int y = 3; // num of people in Y axis
    int arraySize = x*y;
    transmitter::Image imageToSend = transmitter::Image(x, y, transmitter::colors::Red);
    uint8_t* arrayToSend = new u_int8_t[arraySize];
    bool wasSuccess = imageToSend.encode(arrayToSend, arraySize);

    struct ftdi_context *ftdi; // ftdi context for transmitter we are using
    char letter;    // current letter user entered
    int i = 0;      // basic looping variable used throughout code
    int nbytes = 0; // number of bytes a package sent (global variable)
    

    if (setupFTDIConnection(ftdi) != 0) {
        return EXIT_FAILURE;
    }

    printf("broadcasting.\n");

    // open curses session for display purposes
    std::cout << "Hello, welcome to Ben's Halftime Toolkit!\n"
              << "a=twinkle routine; r=red flash; e=green flash; b=blue flash\n"
              << "d=dark; g=gold flash;  w=white flash; t=test\n"
              << "n=gold on; o=white on; v=orange\n"
              << "m=magenta; y=yellow flash;  k=cyan flash\n"
              << "c=christmas sparkle;  f=twink9; h=twink9; j=twink10; l=twink11\n"
              << "s=rainbow short;  p=rainbow med\n"
              << "q=sparkle; x=XMas solid; z=slow twinkle\n"
              << "[=marqee one way; ]=marqee the other;\n"
              << "(=slow marqee one way; )=slow marqee the other;\n\n"
              << " comma key <,> to stop loop\n"
              << " dot key <.> to quit\n";

    system("stty raw"); // enter raw mode so no newline is required for getchar

    // loop until a '.' character is detected
    do {
        letter = getchar();
        switch (letter) {

        case 'c': // christmas sparkle
            do {
                nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
                usleep(SLP);
            } while (getchar() != ',');
            break;

        case 'p': // rainbow med
            nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
            usleep(DASH);
            nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
            usleep(DAB);
            nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
            usleep(DASH);
            nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
            usleep(DAB);
            nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
            usleep(DASH);
            nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
            usleep(DAB);
            nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
            usleep(DASH);
            nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
            usleep(DAB);
            nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
            break;

        default:
            usleep(DAB);
        }
        nbytes = ftdi_write_data(ftdi, arrayToSend, arraySize);
        // Draw a space over current character
    } while (letter != '.');

    if (closeFTDIConnection(ftdi) != 0) {
        return EXIT_FAILURE;
    }

    printf("End of program.\n");
    delete[] arrayToSend;

    return EXIT_SUCCESS;
}

int setupFTDIConnection(ftdi_context* ftdi) {
    int numDevicesFound = 0; // number of FTDI devices found on the machine
    struct ftdi_device_list *devlist, *curdev; // list of ftdi devices
    char manufacturer[128]; // name of manufacturer of transmitter
    char description[128];  // description of transmistter
    int errors; // used throughout to check if there were any errors with FTDI stuff

    // initialize new ftdi device
    if ((ftdi = ftdi_new()) == 0) {
        fprintf(stderr, "ftdi_new failed\n");
        return 1;
    } else {
        fprintf(stderr, "ftdi_new success\n");
    }

    // detect connected ftdi device(s)
    if ((numDevicesFound = ftdi_usb_find_all(ftdi, &devlist, 0x0403, 0x6001)) < 0) {
        fprintf(stderr, "no ftdi devices found\n");
        fflush(stderr);
        ftdi_list_free(&devlist);
        ftdi_free(ftdi);
        return 1;
    } else {
        fprintf(stderr, "%d ftdi devices found.\n", numDevicesFound);
    }

    // loop through detected devices and attempt to get their information
    int i = 0;
    for (curdev = devlist; curdev != NULL; i++) {
        printf("Checking device: %d\n", i);
        if ((errors = ftdi_usb_get_strings(ftdi, curdev->dev, manufacturer, 128,
                                        description, 128, NULL, 0)) < 0) {
            fprintf(stderr, "ftdi_usb_get_strings failed: %d (%s)\n", errors,
                    ftdi_get_error_string(ftdi));
            ftdi_list_free(&devlist);
            ftdi_free(ftdi);
            return errors;
        }
        printf("Manufacturer: %s, Description: %s\n\n", manufacturer,
               description);
        curdev = curdev->next;
    }

    // open ftdi context
    if ((errors = ftdi_usb_open_dev(ftdi, devlist->dev)) < 0) {
        fprintf(stderr, "unable to open ftdi: %d (%s)\n", errors,
                ftdi_get_error_string(ftdi));
        ftdi_free(ftdi);
        return errors;
    } else {
        fprintf(stderr, "ftdi_open successful\n");
    }

    // set the base bitrate/baudrate of the device(s)
    errors = ftdi_set_baudrate(ftdi, 57600);
    if (errors < 0) {
        fprintf(stderr, "unable to set baud rate: %d (%s).\n", errors,
                ftdi_get_error_string(ftdi));
        return errors;
    } else {
        printf("baudrate set.\n");
    }

    // set parameters in the devices
    errors = ftdi_set_line_property(ftdi, (ftdi_bits_type) 8, STOP_BIT_1, NONE);
    if (errors < 0) {
        fprintf(stderr, "unable to set line parameters: %d (%s).\n", errors,
                ftdi_get_error_string(ftdi));
        return errors;
    } else {
        printf("line parameters set.\n");
    }

    ftdi_list_free(&devlist);
    return 0;
}

int closeFTDIConnection(ftdi_context* ftdi) {
    // close ftdi device connection
    int errors; // errors from ftdi function calls
    if ((errors = ftdi_usb_close(ftdi)) < 0) {
        fprintf(stderr, "unable to close ftdi1: %d (%s)\n", errors,
                ftdi_get_error_string(ftdi));
    }
    ftdi_free(ftdi);
    return errors;
}
