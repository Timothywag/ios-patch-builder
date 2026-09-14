#include <mach/mach.h>
#include <mach-o/dyld.h>
#include <stdint.h>
#include <string.h>

static void patch_memory(uintptr_t target_addr, const uint8_t *bytes, size_t size) {
    kern_return_t kr = vm_protect(
        mach_task_self(), 
        (vm_address_t)target_addr, 
        size, 
        0, 
        VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY
    );
    if (kr == KERN_SUCCESS) {
        memcpy((void *)target_addr, bytes, size);
        vm_protect(
            mach_task_self(), 
            (vm_address_t)target_addr, 
            size, 
            0, 
            VM_PROT_READ | VM_PROT_EXECUTE
        );
    }
}

__attribute__((constructor))
static void entry(void) {
    uintptr_t slide = _dyld_get_image_vmaddr_slide(0);

    // 0x20008052c0035fd6 -> mov w0, #1; ret (returns true / allowed)
    const uint8_t ret_true[] = {0x20, 0x00, 0x80, 0x52, 0xc0, 0x03, 0x5f, 0xd6};

    // 0x00008052c0035fd6 -> mov w0, #0; ret (returns false)
    const uint8_t ret_false[] = {0x00, 0x00, 0x80, 0x52, 0xc0, 0x03, 0x5f, 0xd6};

    patch_memory(slide + 0x100019e4c, ret_true, sizeof(ret_true));[span_0](start_span)[span_0](end_span)
    patch_memory(slide + 0x10001a238, ret_false, sizeof(ret_false));[span_1](start_span)[span_1](end_span)
    patch_memory(slide + 0x10001b2c4, ret_true, sizeof(ret_true));[span_2](start_span)[span_2](end_span)
}
