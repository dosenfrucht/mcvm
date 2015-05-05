position 0x8000
text:  db "Hello World.", 0x0A, 0x00
text2: db "This text is being printed by a virtual machine.", 0x0A, 0x00

position 0x00
_start:
  mov rdi, text
  call print_string
  mov rdi, text2
  call print_string
  hlt
  
print_string:
  load8 [rdi], rax
  cmp rax, 0
  je print_string.null
  jmp print_string.not_null
  
 print_string.null:
  ret
 
 print_string.not_null:
  mov r9, rdi
  mov rdi, rax
  call put_char
  mov rdi, r9
  add rdi, 1
  jmp print_string


put_char:
  mov r13, rax
  mov rax, rdi
  int 26
  mov rax, r13
  ret
  

