OUTPUT_FORMAT(binary)

ENTRY (main)

INPUT
(
    func.o
)

OUTPUT
(
    func.bin
)

SECTIONS
{
    . = 0x100;
    .text ALIGN(0x100) :
    {
        *(.text)
    }
}
