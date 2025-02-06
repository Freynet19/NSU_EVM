#include <iostream>
#include <libusb.h>

typedef unsigned char uchar;

void printDeviceInfo(libusb_device *dev) {
    libusb_device_descriptor desc;
    int code = libusb_get_device_descriptor(dev, &desc);
    if (code < 0) {
        std::cerr << "Error: failed to get device descriptor, code: " << code << std::endl;
        return;
    }

    std::cout << "Class: " << static_cast<int>(desc.bDeviceClass) << std::endl;
    std::cout << "Vendor ID: " << desc.idVendor << std::endl;
    std::cout << "Product ID: " << desc.idProduct << std::endl;

    libusb_device_handle *handle;
    code = libusb_open(dev, &handle);

    if (code == 0) {
        if (uchar vendor[64];
            libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, vendor, sizeof(vendor))) {
            std::cout << "Vendor: " << vendor << std::endl;
        }
        if (uchar product[64];
            libusb_get_string_descriptor_ascii(handle, desc.iProduct, product, sizeof(product))) {
            std::cout << "Product: " << product << std::endl;
        }
        if (uchar serialNumber[64];
            libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, serialNumber, sizeof(serialNumber))) {
            std::cout << "Serial number: " << serialNumber << std::endl;
        }
        libusb_close(handle);
    }
    else std::cerr << "Error: failed to open device." << std::endl;

    std::cout << "--------------------------" << std::endl;
}

int main() {
    libusb_context *ctx = nullptr;
    if (const int code = libusb_init(&ctx); code < 0) {
        std::cerr << "Error: failed to initialize libusb, code: " << code << std::endl;
        return 1;
    }

    libusb_device **devs;
    const ssize_t cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        std::cerr << "Error: failed to get device list." << std::endl;
        libusb_exit(ctx);
        return 1;
    }

    std::cout << "USB devices found: " << cnt << std::hex << std::endl;
    std::cout << "==========================" << std::endl;

    for (ssize_t i = 0; i < cnt; i++) {
        printDeviceInfo(devs[i]);
    }

    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);
    return 0;
}