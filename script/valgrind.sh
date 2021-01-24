#cd ../build/cmake/dev/src
EXEC=../build/cmake/dev/src/openwinch

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         --suppressions=valgrind-supress.txt \
         $EXEC