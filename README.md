Template GD32F303CG / SPL project

Board based on the ```WeAct Studio BluePill-Plus GD32F303CC``` development board.

This project uses a GD32F303CG MCU rather than a GD32F303CC since some of the original boards had processors with [non-working FPUs](https://github.com/WeActTC/BluePill-Plus/issues/4).
These were manually repaired with a [GD32F303CG](https://lcsc.com/product-detail/Microcontroller-Units-MCUs-MPUs-SOCs_GigaDevice-Semicon-Beijing-GD32F303CGT6_C2835084.html) part.

Example usage:

i.  Create build system: ```cmake -DCMAKE_BUILD_TYPE=Debug -B build```

ii. Build firmware: ```cmake --build build --target all```
