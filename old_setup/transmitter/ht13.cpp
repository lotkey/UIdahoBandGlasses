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

#include <ftdi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
/* #include <time.h> */
/* #include <sys/times.h> */
// #include <curses.h>

#include "packs.hpp"

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
long millisec();
uint8_t sorc[96] = {};
uint8_t dest[96] = {};

void rotate13(uint8_t arr[]);

int main() {
    struct ftdi_context *ftdi; // ftdi context for transmitter we are using
    struct ftdi_device_list *devlist, *curdev;
    char manufacturer[128]; // name of manufacturer of transmitter
    char description[128];  // description of transmistter
    char letter;    // current letter user entered
    int i = 0;      // basic looping variable used throughout code
    int nbytes = 0; // number of bytes a package sent (global variable)

    int ret = 0; // used throughout code to provide a return value for main()
    int res;     // ftdi stuff
    int f;       // used for setting parameters to ftdi
    
    size_t l = sizeof(dest);
    size_t m = getEncodedBufferSize(l);

    // initialize new ftdi device
    if ((ftdi = ftdi_new()) == 0) {
        fprintf(stderr, "ftdi_new failed\n");
        return EXIT_FAILURE;
    } else {
        fprintf(stderr, "ftdi_new success\n");
    }

    // detect connected ftdi device(s)
    if ((res = ftdi_usb_find_all(ftdi, &devlist, 0x0403, 0x6001)) < 0) {
        fprintf(stderr, "no ftdi devices found\n");
        fflush(stderr);
        ftdi_list_free(&devlist);
        ftdi_free(ftdi);
        return 1;
    } else {
        fprintf(stderr, "%d ftdi devices found.\n", res);
    }

    // loop through detected devices and attempt to get their information
    i = 0;
    for (curdev = devlist; curdev != NULL; i++) {
        printf("Checking device: %d\n", i);
        if ((ret = ftdi_usb_get_strings(ftdi, curdev->dev, manufacturer, 128,
                                        description, 128, NULL, 0)) < 0) {
            fprintf(stderr, "ftdi_usb_get_strings failed: %d (%s)\n", ret,
                    ftdi_get_error_string(ftdi));
            ftdi_list_free(&devlist);
            ftdi_free(ftdi);
            // Added this line, not sure if this causes an issue
            return EXIT_FAILURE;
        }
        printf("Manufacturer: %s, Description: %s\n\n", manufacturer,
               description);
        curdev = curdev->next;
    }

    // open ftdi context
    if ((ret = ftdi_usb_open_dev(ftdi, devlist->dev)) < 0) {
        fprintf(stderr, "unable to open ftdi: %d (%s)\n", ret,
                ftdi_get_error_string(ftdi));
        ftdi_free(ftdi);
        return ret;
    } else {
        fprintf(stderr, "ftdi_open successful\n");
    }

    // set the base bitrate/baudrate of the device(s)
    ret = ftdi_set_baudrate(ftdi, 57600);
    if (ret < 0) {
        fprintf(stderr, "unable to set baud rate: %d (%s).\n", ret,
                ftdi_get_error_string(ftdi));
    } else {
        printf("baudrate set.\n");
    }

    // set parameters in the devices
    f = ftdi_set_line_property(ftdi, (ftdi_bits_type) 8, STOP_BIT_1, NONE);
    if (f < 0) {
        fprintf(stderr, "unable to set line parameters: %d (%s).\n", ret,
                ftdi_get_error_string(ftdi));
    } else {
        printf("line parameters set.\n");
    }

    ftdi_list_free(&devlist);
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
                nbytes = ftdi_write_data(ftdi, X1Pack, m);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, dPack, m);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, X2Pack, m);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, dPack, m);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, X3Pack, m);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, dPack, m);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, X4Pack, m);
                usleep(SLP);
                nbytes = ftdi_write_data(ftdi, dPack, m);
                usleep(SLP);
            } while (getchar() != ',');
            break;

        case 'p': // rainbow med
            nbytes = ftdi_write_data(ftdi, rain5Pack, m);
            usleep(DASH);
            nbytes = ftdi_write_data(ftdi, rain6Pack, m);
            usleep(DAB);
            nbytes = ftdi_write_data(ftdi, rain6Pack, m);
            usleep(DASH);
            nbytes = ftdi_write_data(ftdi, rain7Pack, m);
            usleep(DAB);
            nbytes = ftdi_write_data(ftdi, rain7Pack, m);
            usleep(DASH);
            nbytes = ftdi_write_data(ftdi, rain8Pack, m);
            usleep(DAB);
            nbytes = ftdi_write_data(ftdi, rain8Pack, m);
            usleep(DASH);
            nbytes = ftdi_write_data(ftdi, dPack, m);
            usleep(DAB);
            nbytes = ftdi_write_data(ftdi, dPack, m);
            break;

        case ']': // fast marqee right
            for (i = 0; i < 96; i++) {
                sorc[i] = igPack[i];
            }

            do {
                for (i = 93; i < 96; i++) {
                    temp[i - 93] = sorc[i];
                }
                for (i = 95; i > -1; i--) {
                    if (i > 2)
                        sorc[i] = sorc[i - 3];
                    else
                        sorc[i] = temp[i];
                }
                nbytes = ftdi_write_data(ftdi, sorc, m);
                usleep(DAB);
            } while (getchar() != ',');
            break;

        case '{': // slow marqee left
            // load the broadcast array
            for (i = 0; i < 96; i++) {
                sorc[i] = igPack[i];
            }
            // loop
            do {
                // store the rollover values
                for (i = 0; i < 3; i++) {
                    temp[i] = sorc[i];
                }
                for (i = 0; i < 96; i++) {
                    if (i < 93)
                        sorc[i] = sorc[i + 3];
                    else
                        sorc[i] = temp[i % 3];
                }
                nbytes = ftdi_write_data(ftdi, sorc, m);
                usleep(SLOW);
            } while (getchar() != ',');
            break;

        default:
            usleep(DAB);
        }
        nbytes = ftdi_write_data(ftdi, dPack, m);
        // Draw a space over current character
    } while (letter != '.');

    // close ftdi device connection
    if ((ret = ftdi_usb_close(ftdi)) < 0) {
        fprintf(stderr, "unable to close ftdi1: %d (%s)\n", ret,
                ftdi_get_error_string(ftdi));
        return EXIT_FAILURE;
    }

    ftdi_free(ftdi);

    printf("End of program.\n");

    return EXIT_SUCCESS;
}


static size_t getEncodedBufferSize(size_t sourceSize) {
    size_t s;
    s = sourceSize + sourceSize / 254 + 1;
    // printf("buffer size is : %zd.\n", s);
    return s;
}
