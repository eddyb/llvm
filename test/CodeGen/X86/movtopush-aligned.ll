; RUN: llc < %s -mtriple=i686-windows -stackrealign -stack-alignment=32 | FileCheck %s -check-prefix=ALIGNED

declare void @good(i32 %a, i32 %b, i32 %c, i32 %d)
declare void @oneparam(i32 %a)
declare void @eightparams(i32 %a, i32 %b, i32 %c, i32 %d, i32 %e, i32 %f, i32 %g, i32 %h)

; When there is no reserved call frame, check that additional alignment
; is added when the pushes don't add up to the required alignment.
; ALIGNED-LABEL: test5:
; ALIGNED: subl    $16, %esp
; ALIGNED-NEXT: pushl   $4
; ALIGNED-NEXT: pushl   $3
; ALIGNED-NEXT: pushl   $2
; ALIGNED-NEXT: pushl   $1
; ALIGNED-NEXT: call
define void @test5(i32 %k) {
entry:
  %a = alloca i32, i32 %k
  call void @good(i32 1, i32 2, i32 3, i32 4)
  ret void
}

; When the alignment adds up, do the transformation
; ALIGNED-LABEL: test5b:
; ALIGNED: pushl   $8
; ALIGNED-NEXT: pushl   $7
; ALIGNED-NEXT: pushl   $6
; ALIGNED-NEXT: pushl   $5
; ALIGNED-NEXT: pushl   $4
; ALIGNED-NEXT: pushl   $3
; ALIGNED-NEXT: pushl   $2
; ALIGNED-NEXT: pushl   $1
; ALIGNED-NEXT: call
define void @test5b() optsize {
entry:
  call void @eightparams(i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8)
  ret void
}

; When having to compensate for the alignment isn't worth it,
; don't use pushes.
; ALIGNED-LABEL: test5c:
; ALIGNED: movl $1, (%esp)
; ALIGNED-NEXT: call
define void @test5c() optsize {
entry:
  call void @oneparam(i32 1)
  ret void
}
