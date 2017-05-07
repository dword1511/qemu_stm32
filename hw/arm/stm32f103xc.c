#include "hw/arm/stm32.h"
#include "hw/sysbus.h"
#include "hw/arm/arm.h"
#include "hw/devices.h"
#include "ui/console.h"
#include "sysemu/sysemu.h"
#include "hw/boards.h"

static void stm32_board_init(MachineState *machine) {
  const char* kernel_filename = machine->kernel_filename;

  stm32_init( /* Flash */ 0x0003ffff,
              /* RAM   */ 0x0000ffff,
                          kernel_filename,
              /* HSE */   8000000,
              /* LSE */   32768);

  DeviceState *uart2 = DEVICE(object_resolve_path("/machine/stm32/uart[2]", NULL));
  DeviceState *uart1 = DEVICE(object_resolve_path("/machine/stm32/uart[1]", NULL));
  DeviceState *uart3 = DEVICE(object_resolve_path("/machine/stm32/uart[3]", NULL));
  assert(uart2);
  assert(uart1);
  assert(uart3);

  /* Connect RS232 to UART */
  stm32_uart_connect(
      (Stm32Uart *)uart2,
      serial_hds[0],
      STM32_USART2_NO_REMAP);

  stm32_uart_connect(
      (Stm32Uart *)uart1,
      serial_hds[0],
      STM32_USART1_NO_REMAP);

  stm32_uart_connect(
      (Stm32Uart *)uart3,
      serial_hds[0],
      STM32_USART3_NO_REMAP);
}

static QEMUMachine stm32_machine = {
  .name = "stm32f103xc",
  .desc = "STM32F103xC minimal system (256 KB Flash, 64 KB RAM)",
  .init = stm32_board_init,
};

static void stm32_machine_init(void) {
  qemu_register_machine(&stm32_machine);
}

machine_init(stm32_machine_init);
