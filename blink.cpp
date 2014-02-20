/*
  
  (C) 2014 by Alexander Kasimir
  
*/ 

#include <stdio.h>
#include <string.h>
#if defined WIN
  #include <lusb0_usb.h>    // this is libusb, see http://libusb.sourceforge.net/
#else
  #include <usb.h>        // this is libusb, see http://libusb.sourceforge.net/
#endif

int main (int argc, char **argv)
{

  bool sendLine = true;
  bool debug = false;
  int arg_pointer = 1;
  char *output = NULL;

  char r=0x00;
  char g=0x00;
  char b=0x00;

  if      (argc == 2 && strcmp(argv[1],"red")    == 0 ) { r= 0xff; g=0x00; b=0x00; }
  else if (argc == 2 && strcmp(argv[1],"green")  == 0 ) { r= 0x00; g=0xff; b=0x00; }
  else if (argc == 2 && strcmp(argv[1],"blue")   == 0 ) { r= 0x00; g=0x00; b=0xff; }
  else if (argc == 2 && strcmp(argv[1],"white")  == 0 ) { r= 0xff; g=0xff; b=0xff; }
  else if (argc == 2 && strcmp(argv[1],"gray")  == 0 )  { r= 0x50; g=0x50; b=0x50; }
  else if (argc == 2 && strcmp(argv[1],"grey")  == 0 )  { r= 0x50; g=0x50; b=0x50; }
  else if (argc == 2 && strcmp(argv[1],"yellow") == 0 ) { r= 0xff; g=0xff; b=0x00; }
  else if (argc == 2 && strcmp(argv[1],"pink")   == 0 ) { r= 0xff; g=0x00; b=0xff; }
  else if (argc == 2 && strcmp(argv[1],"cyan")   == 0 ) { r= 0x00; g=0xff; b=0xff; }
  else if (argc == 2 && strcmp(argv[1],"black")  == 0 ) { r= 0x00; g=0x00; b=0x00; }
  else if (argc == 2 && strcmp(argv[1],"off")    == 0 ) { r= 0x00; g=0x00; b=0x00; }
  else if (argc < 4) {
    printf("Usage blink red green blue...\n");
    return 0;
  } else {
    r = atoi(argv[1]) & 0xFF;
    g = atoi(argv[2]) & 0xFF;
    b = atoi(argv[3]) & 0xFF;
  }

  struct usb_bus *bus = NULL;
  struct usb_device *digiSpark = NULL;
  struct usb_device *device = NULL;

  if(debug) printf("Detecting USB devices...\n");

  // Initialize the USB library
  usb_init();

  // Enumerate the USB device tree
  usb_find_busses();
  usb_find_devices();

  // Iterate through attached busses and devices
  bus = usb_get_busses();
  while(bus != NULL)
  {
     device = bus->devices;
     while(device != NULL)
     {
        // Check to see if each USB device matches the DigiSpark Vendor and Product IDs
        if((device->descriptor.idVendor == 0x16c0) && (device->descriptor.idProduct == 0x05df))
        {
           if(debug) printf("Detected DigiSpark... \n");
           digiSpark = device;
        }

        device = device->next;
     }

     bus = bus->next;
  }

  if(digiSpark != NULL)
  {
     int result = 0;
     int i = 0;
     int stringLength = 0;
     int numInterfaces = 0;
     struct usb_dev_handle *devHandle = NULL;
     struct usb_interface_descriptor *interface = NULL;

     if(argc > 1)
     {

        devHandle = usb_open(digiSpark);         
        if(devHandle != NULL)
        {
           numInterfaces = digiSpark->config->bNumInterfaces;
           interface = &(digiSpark->config->interface[0].altsetting[0]);
           if(debug) printf("Found %i interfaces, using interface %i\n", numInterfaces, interface->bInterfaceNumber);

           usb_control_msg(devHandle, (0x01 << 5), 0x09, 0, 's' , 0, 0, 1000);
           usb_control_msg(devHandle, (0x01 << 5), 0x09, 0, r , 0, 0, 1000);
           usb_control_msg(devHandle, (0x01 << 5), 0x09, 0, g , 0, 0, 1000);
           usb_control_msg(devHandle, (0x01 << 5), 0x09, 0, b , 0, 0, 1000);           

           result = usb_release_interface(devHandle, interface->bInterfaceNumber);
//           if(result < 0) {if(debug) printf("Error %i releasing Interface 0\n", result); return 1;}

           usb_close(devHandle);
        }
     }
  }
  else{
    printf("No Digispark Found");
    return 1;
  }     
        
  return 0;
}