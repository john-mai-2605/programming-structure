# Adapted from https://stackoverflow.com/questions/30559296/diff-command-in-bash
for NUM in 1 2 3 4 5
do
    as --32 -march=i386 mydc.s -o mydc.o
    gcc209 -g -m32 -march=i386 mydc.o -o mydc
    ./mydc < tests/input$NUM.txt > tests/result$NUM.result
    if diff tests/result$NUM.result tests/result$NUM.expected > /dev/null 2>&1
    then
    echo "PASS"
    else
    echo "FAIL"
    fi
done
