#include <iostream>
#include <libusb-1.0/libusb.h>

#define VENDOR_ID 0x1af3
#define PRODUCT_ID 0x0001

class USBMouse {
public:
    USBMouse() {
        if (libusb_init(&context) < 0) {
            throw std::runtime_error("Failed to initialize libusb");
        }

        handle = libusb_open_device_with_vid_pid(context, VENDOR_ID, PRODUCT_ID);
        if (!handle) {
            libusb_exit(context);
            throw std::runtime_error("Failed to open device");
        }

        libusb_detach_kernel_driver(handle, 0);
        if (libusb_claim_interface(handle, 0) < 0) {
            std::cerr << libusb_claim_interface(handle, 0) << std::endl;
            libusb_close(handle);
            libusb_exit(context);
            throw std::runtime_error("Failed to claim interface");
        }
    }

    ~USBMouse() {
        if (handle) {
            libusb_release_interface(handle, 0);
            libusb_attach_kernel_driver(handle, 0);
            libusb_close(handle);
        }
        libusb_exit(context);
    }

    void run() const {
        while (true) {
            unsigned char data[6];
            int transferred;
            int code = libusb_interrupt_transfer(handle, 0x81, data, sizeof(data), &transferred, 0);
            if (code == 0) printMouseData(data);
            else std::cerr << "Data transferring error: " << libusb_error_name(code) << std::endl;
            if (data[0] & 0x01 && data[0] & 0x02 && data[0] & 0x04) {
                std::cout << "LMB + MMB + RMB pressed. Stopping..." << std::endl;
                break;
            }
        }
    }

private:
    libusb_context *context = nullptr;
    libusb_device_handle *handle = nullptr;

    static void printMouseData(const unsigned char *data) {
        int x = data[1] - data[2]; // 1, 3 перемещение (если отрицательно то 255-значение)
        int y = data[3] - data[4]; // 2, 4 принимают значения 0 и 255
        int btn = data[0];
        int scr = data[5];

        if (btn & 0x01) std::cout << "LMB pressed. ";
        if (btn & 0x02) std::cout << "RMB pressed. ";
        if (btn & 0x04) std::cout << "MMB pressed. ";
        if (btn & 0x08) std::cout << "Back pressed. ";
        if (btn & 0x10) std::cout << "Forward pressed. ";
        if (scr == 0x01) std::cout << "Scroll up. ";
        if (scr == 0xff) std::cout << "Scroll down. ";
        std::cout << "Move: (" << x << ", " << y << ")" << std::endl;
    }
};

int main() {
    try {
        USBMouse mouse;
        mouse.run();
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
