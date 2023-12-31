import os
from pathlib import Path

#root_dir = Path(__file__).resolve().parent.parent
root_dir = Path(os.getcwd())
link_ld_path = os.path.join(str(root_dir), r'src/boot/link.ld')

iso_path = "circuit.iso"
target_path = "target/"
srcfonts = "src/fonts/"
binfonts = "bin/fonts/"

env = Environment()

OPT_LEVEL = "0"
STDC = "2x"

env['CC'] = "i386-elf-gcc"
env['CCFLAGS'] = ["-ffreestanding", "-O" + OPT_LEVEL, "-std=c" + STDC, "-Wall", "-Wextra", "-gdwarf"]

env['AS'] = "i386-elf-gcc"
env['ASFLAGS'] = ["-ffreestanding", "-O" + OPT_LEVEL, "-Wall", "-Wextra", "-gdwarf"]

env['LINK'] = "i386-elf-gcc"
env['LINKFLAGS'] = ["-T", link_ld_path, "-ffreestanding", "-nostdlib",  "-lgcc", "-O" + OPT_LEVEL, "-gdwarf"]

env_nasm = Environment()
env_nasm['AS'] = "nasm"
env_nasm['ASFLAGS'] = ["-felf", "-g"]

env.Command(f"{binfonts}lat9-16.o", f"{srcfonts}lat9-16.psf", f"objcopy -O elf32-i386 -B i386 -I binary {srcfonts}lat9-16.psf {binfonts}lat9-16.o")

objects = [
    # boot
    env.Object(target="bin/boot.o", source="src/boot/boot.S"),

    # libraries
    env.Object(target="bin/string.o", source="src/lib/sys/string.c"),
    env.Object(target="bin/rgb.o", source="src/lib/sys/rgb.c"),

    # kernel
    env.Object(target="bin/kernel.o", source="src/kernel/entry.c"),
    env.Object(target="bin/io.o", source="src/kernel/io.c"),
    env.Object(target="bin/terminal.o", source="src/kernel/terminal.c"),
    env.Object(target="bin/shell.o", source="src/kernel/shell.c"),
    env.Object(target="bin/shellcmds.o", source="src/kernel/shellcmds.c"),

    # fonts
    "bin/fonts/lat9-16.o",
]

env.Program(target="target/boot/os.bin", source=objects)

#env.Execute("objcopy -O elf64-x86-64 -B i386 -I binary lat9-16.psf lat9-16.o")

env.Command("target/boot/grub/grub.cfg", "src/boot/grub.cfg", Copy('$TARGET', '$SOURCE'))

env.Command(iso_path, target_path, f"grub-mkrescue -o {iso_path} {target_path}")

