segment .text

global get_vga_font
get_vga_font:
  ; Save registers
  push ax
  push bx

  ; Set AX to 1130h and BH to the desired font
  mov ax, 1130h
  mov bh, 02h ; ROM 8x8 double dot font pointer

  ; Call the BIOS interrupt
  int 10h

  ; Move the segment to EAX
  movzx eax, bp

  ; Shift left to make room for the offset
  shl eax, 16

  ; Add the offset to EAX
  add eax, es

  ; Restore registers and return
  pop bx
  pop ax
  ret
