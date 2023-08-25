#pragma once

void _main();

extern "C" __attribute__((section(".startup"))) void startup() {
    asm volatile("_startup_main:");
    _main();
    asm volatile("_startup_main_end:");
}