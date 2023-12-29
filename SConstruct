import os
from pathlib import Path

#root_dir = Path(__file__).resolve().parent.parent
root_dir = Path(os.getcwd())
link_ld_path = os.path.join(str(root_dir), r'src/boot/link.ld')

iso_path = "circuit.iso"
target_path = "target/"

env = Environment()

env['CC'] = "i386-elf-gcc"
env['CCFLAGS'] = ["-ffreestanding", "-O0", "-Wall", "-Wextra"]

env['AS'] = "i386-elf-as"

env['LINK'] = "i386-elf-gcc"
env['LINKFLAGS'] = ["-T", link_ld_path, "-ffreestanding", "-nostdlib",  "-lgcc", "-O0"]

objects = [
    # boot
    env.Object(target="bin/boot.o", source="src/boot/boot.s"),

    # libraries
    env.Object(target="bin/string.o", source="src/lib/sys/string.c"),

    # kernel
    env.Object(target="bin/kernel.o", source="src/kernel/entry.c"),
    env.Object(target="bin/vga.o", source="src/kernel/vga.c"),
    env.Object(target="bin/terminal.o", source="src/kernel/terminal.c"),
]

env.Program(target="target/boot/os.bin", source=objects)

env.Command("target/boot/grub/grub.cfg", "src/boot/grub.cfg", Copy('$TARGET', '$SOURCE'))

env.Command(iso_path, target_path, f"grub-mkrescue -o {iso_path} {target_path}")

