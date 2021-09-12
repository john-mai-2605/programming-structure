### --------------------------------------------------------------------
### mydc.s
###
### Desk Calculator (dc)
### --------------------------------------------------------------------

	.equ   ARRAYSIZE, 20
	.equ   EOF, -1
        .equ   UNDERSCORE, 95
        .equ   MINUS, 45
        .equ   PLUS, 43
        .equ   MUL, 42
        .equ   DIV, 47
        .equ   PRINT, 112
        .equ   QUIT, 113
        .equ   REPLACE, 114
        .equ   CLEAR, 99
        .equ   DOUBLE, 100
        .equ   FSTACK, 102
        .equ   ABSSUM, 124
        .equ   ZERO, 0
        .equ   FALSE, 0
        .equ   OFS, 4
        .equ   TWO_OFS, 8
        .equ   THREE_OFS, 12
	
.section ".rodata"

scanfFormat:
	.asciz "%s"
printfEmptyFormat:
        .asciz "dc: stack empty\n"
printfDZFormat:
        .asciz "dc: Division by zero. Proceed\n"
printfTopFormat:
        .asciz "%d\n"
### --------------------------------------------------------------------

        .section ".data"

### --------------------------------------------------------------------

        .section ".bss"
buffer:
        .skip  ARRAYSIZE

### --------------------------------------------------------------------

	.section ".text"

	## -------------------------------------------------------------
	## int main(void)
	## Runs desk calculator program.  Returns 0.
	## -------------------------------------------------------------

	.globl  main
	.type   main,@function

main:

	pushl   %ebp
	movl    %esp, %ebp

input:

	## dc number stack initialized. %esp = %ebp
	
	## scanf("%s", buffer)
	pushl	$buffer
	pushl	$scanfFormat
	call    scanf
	addl    $TWO_OFS, %esp

	## check if user input EOF
	cmpl	$EOF, %eax
	je	quit

        /* is this line a number? */
        /* Pseudo code: 
        if (isdigit(buffer[0]))
                goto number;
        */
        movl    $ZERO, %eax
        movl    $ZERO, %ebx
        movb    buffer, %bl
        pushl   %ebx
        movl    $ZERO, %ebx
        call    isdigit
        addl    $OFS, %esp
        
        cmpl    $FALSE, %eax
        jne     number

        /* Pseudo code: 
        if (buffer[0] == '_')
                goto underscore;
        */

        cmpb    $UNDERSCORE, buffer
        je      underscore

        /* is this line p command? */
        /* Pseudo code: 
        if (buffer[0] == p)
                goto p_command;
        */
        cmpb    $PRINT, buffer
        je      p_command

        /* q command, quit */
        /* Pseudo code: 
        if (buffer[0] == 'q')
                goto quit;
        */
        cmpb    $QUIT, buffer
        je      quit

        /* + command */
        /* Pseudo code: 
        if (buffer[0] == '+')
                goto plus_command;
        */
        cmpb    $PLUS, buffer
        je      plus_command

        /* - command */
        /* Pseudo code: 
        if (buffer[0] == '-')
                goto minus_command;
        */
        cmpb    $MINUS, buffer
        je      minus_command

        /* * command */
        /* Pseudo code: 
        if (buffer[0] == '*')
                goto mul_command;
        */
        cmpb    $MUL, buffer
        je      mul_command

        /* / command */
        /* Pseudo code: 
        if (buffer[0] == '/')
                goto div_command;
        */
        cmpb    $DIV, buffer
        je      div_command
    
        /* | command */
        /* Pseudo code: 
        if (buffer[0] == '|')
                goto abssum_command;
        */
        cmpb    $ABSSUM, buffer
        je      abssum_command

        /* f command */
        /* Pseudo code: 
        if (buffer[0] == '-')
                goto f_command;
        */
        cmpb    $FSTACK, buffer
        je      f_command

        /* c command */
        /* Pseudo code: 
        if (buffer[0] == 'c')
                goto c_command;
        */
        cmpb    $CLEAR, buffer
        je      c_command

        /* d command */
        /* Pseudo code: 
        if (buffer[0] == 'd')
                goto d_command;
        */
        cmpb    $DOUBLE, buffer
        je      d_command

        /* r command */
        /* Pseudo code: 
        if (buffer[0] == 'r')
                goto r_command;
        */
        cmpb    $REPLACE, buffer
        je      r_command

        /* Ignore for other keys:
        Pseudo code: 
        goto input;
        */

        jmp     input

	## PSEUDO-CODE
	## /*
	##  * In this pseudo-code we assume that you do not use no local variables
	##  * in the _main_ process stack. In case you want to allocate space for local
	##  * variables, please remember to update logic for 'empty dc stack' condition
	##  * (stack.peek() == NULL). 
	##  */
	##
        ##  while (1) {
        ##     /* read the current line into buffer */
        ##     if (scanf("%s", buffer) == EOF)
        ##         return 0;
        ## 
        ##     /* is this line a number? */
        ##     if (isdigit(buffer[0]) || buffer[0] == '_') {
        ##        int num;
        ##        if (buffer[0] == '_') buffer[0] = '-';
        ##        num = atoi(buffer);
        ##        stack.push(num);	/* pushl num */
        ##        continue;
        ##     }
        ## 
        ##     /* p command */
        ##     if (buffer[0] == 'p') {
        ##        if (stack.peek() == NULL) { /* is %esp == %ebp? */
        ##           printf("dc: stack empty\n");
        ##        } else {
        ##           /* value is already pushed in the stack */
        ##           printf("%d\n", (int)stack.top()); 
        ##        }
        ##        continue;
        ##     }
        ## 
        ##     /* q command */
        ##     if (buffer[0] == 'q') {
        ##        goto quit;
        ##     }
        ##  
        ##     /* + operation */
        ##     if (buffer[0] == '+') {
        ##        int a, b;
        ##        if (stack.peek() == NULL) {
        ##           printf("dc: stack empty\n");
        ##           continue;
        ##         }
        ##         a = (int)stack.pop();
        ##         if (stack.peek() == NULL) {
        ##            printf("dc: stack empty\n");
        ##            stack.push(a); /* pushl some register value */
        ##            continue;
        ##         }
        ##         b = (int)stack.pop(); /* popl to some register */
        ##         res = a + b;
        ##         stack.push(res);
        ##         continue;
        ##     }
        ## 
        ##     /* - operation */
        ##     if (buffer[0] == '-') {
        ##        /* ... */
        ##     }
        ## 
        ##     /* | operation */
        ##     if (buffer[0] == '|') {
        ##        /* pop two values & call abssum() */
        ##     }
        ## 
        ##     /* other operations and commands */
        ##     if (/* others */) {
        ##        /* ... and so on ... */
        ##     }
        ##  
        ##   } /* end of while */
        ## 

/* Handle number case */
/* Pseudo code: 
        stack.push(atoi(buffer));
        goto input;
*/
number:
        pushl   $buffer
        call    atoi
        addl    $OFS, %esp
        pushl   %eax
        jmp     input

/* Handle underscore case */
/* Pseudo code:
        buffer[0] = '-';
        stack.push(atoi(buffer));
        goto input;
*/
underscore:
        movb    $MINUS, buffer
        pushl   $buffer
        call    atoi
        addl    $OFS, %esp
        pushl   %eax
        jmp     input

/* Handle p case */
/* Pseudo code:
        if (stack.peek() == NULL) goto stack_empty
        else printf("%d\n", (int)stack.top());
*/
p_command:
        cmpl    %esp, %ebp
        je      stack_empty
        pushl   $printfTopFormat
        call    printf
        addl    $OFS, %esp
        jmp     input

/* Handle + case */
/* Pseudo code:
        int a, b;
        if (stack.peek() == NULL) goto stack_empty
        else a = (int)stack.pop();
        if (stack.peek() == NULL) goto stack_empty
        else b = (int)stack.pop();
        stack.push(a+b)
*/
plus_command:
        cmpl    %esp, %ebp
        je      stack_empty
        popl    %ebx
        cmpl    %esp, %ebp
        je      stack_empty
        popl    %ecx
        addl    %ebx, %ecx
        pushl   %ecx
        movl    $ZERO, %ecx
        movl    $ZERO, %ebx
        jmp     input

/* Handle + case */
/* Pseudo code:
        int a, b;
        if (stack.peek() == NULL) goto stack_empty
        else a = (int)stack.pop();
        if (stack.peek() == NULL) goto stack_empty
        else b = (int)stack.pop();
        stack.push(a-b)
*/

minus_command:
        cmpl    %esp, %ebp
        je      stack_empty
        popl    %ebx
        cmpl    %esp, %ebp
        je      stack_empty
        popl    %ecx
        subl    %ebx, %ecx
        pushl   %ecx
        movl    $ZERO, %ecx
        movl    $ZERO, %ebx
        jmp     input

/* Handle * case */
/* Pseudo code:
        int a, b;
        if (stack.peek() == NULL) goto stack_empty
        else a = (int)stack.pop();
        if (stack.peek() == NULL) goto stack_empty
        else b = (int)stack.pop();
        stack.push(a*b)
*/
mul_command:
        cmpl    %esp, %ebp
        je      stack_empty
        popl    %ebx
        cmpl    %esp, %ebp
        je      stack_empty
        popl    %eax
        imull   %ebx
        pushl   %eax
        movl    $ZERO, %ebx
        movl    $ZERO, %eax
        jmp     input

/* Handle / case */
/* Pseudo code:
        int a, b;
        if (stack.peek() == NULL) goto stack_empty
        else a = (int)stack.pop();
        if (stack.peek() == NULL) goto stack_empty
        else b = (int)stack.pop();
        if (b == 0) goto dbz;
        stack.push(a/b)
*/
div_command:
        cmpl    %esp, %ebp
        je      stack_empty
        popl    %ebx
        cmpl    %esp, %ebp
        je      stack_empty
        popl    %eax
        cmpl    $ZERO, %ebx
        je      dbz
        cdq
        idivl   %ebx
        pushl   %eax
        movl    $ZERO, %ebx
        movl    $ZERO, %eax
        jmp     input

/* Division by zero */
/* Pseudo-code
        printf("dc: Division by zero. Proceed\n");
*/

dbz:
        pushl   $printfDZFormat
        call    printf
        addl    $OFS, %esp
        jmp     input
/* Handle | case */
/* Pseudo code:
        int a, b;
        if (stack.peek() == NULL) goto stack_empty
        else a = (int)stack.pop();
        if (stack.peek() == NULL) goto stack_empty
        else b = (int)stack.pop();
        stack.push(abssum(a,b))
*/
abssum_command:
        cmpl    %esp, %ebp
        je      stack_empty
        popl    %ebx
        cmpl    %esp, %ebp
        je      stack_empty
        popl    %ecx
        
        pushl   %ebx
        pushl   %ecx

        call    abssum

        addl    $TWO_OFS, %esp
        
        pushl   %eax
        movl    $ZERO, %eax
        jmp     input

/* Function call abssum() */
/* Pseudo code:
 
int abssum(int a, int b) {
                sum = 0;
        min_max:       
                int min, max = 0;
                min = a; max = b;
                if (a < b) goto less;
                else goto loop_init;d
        less:
                min = b; max = a;
                goto loop_init;
        
        loop_init:
                if (min <= max) goto loop_check;
                else goto done;
        
        loop_check:
                int amin = min;
                if (amin < 0) goto make_abs;
                else goto loop_main;
        
        make_abs:
                int amin = -min;
                goto loop_main;
        
        loop_main:
                sum += aminl
                min++;
                if (min <= max) goto loop_check;
                else goto done;

        done:
                return sum;        
     }
  
*/
.type   abssum,@function
abssum:
        # prolog
        pushl   %ebp
        movl    %esp, %ebp
        jmp     min_max
        ret
min_max:
        # find min_max:
        movl    TWO_OFS(%ebp), %ebx
        movl    THREE_OFS(%ebp), %ecx
        cmpl    %ebx, %ecx
        jl      less
        jmp     loop_init
less:
        movl    THREE_OFS(%ebp), %ebx
        movl    TWO_OFS(%ebp), %ecx
        jmp     loop_init

loop_init:
        # initial state
        movl    $ZERO, %eax
        cmpl    %ecx, %ebx
        jle     loop_check
        jmp     done

loop_check:
        # check negative
        movl    %ebx, %edx
        cmpl    $ZERO, %edx
        jl      make_abs
        jmp     loop_main

make_abs:
        # negation to negative number
        negl    %edx
        jmp     loop_main

loop_main:      
        # main loop
        addl    %edx, %eax
        incl    %ebx
        cmpl    %ecx, %ebx
        jle     loop_check
        jmp     done
        
done:
        # epilog
        movl    %ebp, %esp
        popl    %ebp
        ret

/* Handle f case */
/* Pseudo code:
        int* b = stack.top();

main_f_loop:
        if (b == stack.bottom()) goto input;
        printf("%d\n", *b)
        b--;
        goto main_f_loop;
*/
f_command:
        movl    %esp, %ebx
        jmp     main_f_loop

main_f_loop:
        cmpl    %ebp, %ebx
        je      input
        
        pushl   (%ebx)
        pushl   $printfTopFormat
        call    printf
        addl    $TWO_OFS, %esp

        addl    $OFS, %ebx
        jmp     main_f_loop

/* Handle d case */
/* Pseudo code:
        if (stack.peek() == NULL) goto stack_empty
        stack.top() == stack.bottom();
*/
c_command:
        cmpl    %esp, %ebp
        je      stack_empty
        movl    %ebp, %esp
        jmp     input

/* Handle d case */
/* Pseudo code:
        if (stack.peek() == NULL) goto stack_empty
        stack.push(stack.pop());
*/
d_command:
        cmpl    %esp, %ebp
        je      stack_empty
        pushl   (%esp)
        jmp     input

/* Handle r case */
/* Pseudo code:
        int a, b;
        if (stack.peek() == NULL) goto stack_empty
        else a = (int)stack.pop();
        if (stack.peek() == NULL) goto stack_empty
        else b = (int)stack.pop();
        stack.push(a);
        stack.push(b);
*/
r_command:
        cmpl    %esp, %ebp
        je      stack_empty
        popl    %ebx
        cmpl    %esp, %ebp
        je      stack_empty
        popl    %ecx
        pushl   %ebx
        pushl   %ecx
        movl    $ZERO, %ebx
        movl    $ZERO, %ecx
        jmp     input

/* Handle empty stack */
/* Pseudo code:
        printf("dc: stack empty\n");
*/

stack_empty:
        pushl   $printfEmptyFormat
        call    printf
        addl    $OFS, %esp
        jmp     input

quit:	
	## return 0
	movl    $0, %eax
	movl    %ebp, %esp
	popl    %ebp
	ret
