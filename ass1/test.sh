DIR=".test_1521"
DIR_C=${DIR}/"life.c"
DIR_C_RUN=${DIR}/"life"
DIR_MIPS=${DIR}/"life.s"
DIR_C_OUT=${DIR}/"c.txt"
DIR_MIPS_OUT=${DIR}/"s.txt"
DIR_C_INCLUDE=${DIR}/"cinclude.h"
DIR_MIPS_INCLUDE=${DIR}/"sinclude.s"
DIR_DIFF="diff.txt"

GEN_BOARD_PYTHON_TEXT="
import sys
import numpy as np

out_h = sys.argv[1]
out_mips = sys.argv[2]
size = int(sys.argv[3])

# 2d true/false initial board raw data
data = np.random.choice(a=['0', '1'], size=(size, size), p=[0.5, 0.5])

# export to c header file
with open(out_h, 'w') as f:
    f.write(f'#define NN {size}\n')
    f.write('int N = NN;\n')
    f.write('char board[NN][NN] = {\n')
    for line in data:
        f.write('   {')
        f.write(','.join(line))
        f.write('},\n')
    f.write('};\n')
    f.write('char newboard[NN][NN];\n')
        
# export to mips include file
with open(out_mips, 'w') as f:
    f.write('   .data\n')
    f.write(f'N:	.word {size}\n')
    f.write('board:\n')
    for line in data:
        f.write('   .byte ')
        f.write(', '.join(line))
        f.write('\n')
    f.write(f'newBoard: .space {size * size}')
"


# test and compare difference
# arg1: size
function test() {
    # generate boards, write to files
    local SIZE=$1
    python3 -c "${GEN_BOARD_PYTHON_TEXT}" ${DIR_C_INCLUDE} ${DIR_MIPS_INCLUDE} ${SIZE}
    cat ${DIR_C_INCLUDE} life.c > ${DIR_C}
    cat ${DIR_MIPS_INCLUDE} prog.s > ${DIR_MIPS}

    # compile and run c version
    gcc ${DIR_C} -o ${DIR_C_RUN}
    ./${DIR_C_RUN} <<< ${SIZE} > ${DIR_C_OUT}

    # run mips version
    1521 spim -file ${DIR_MIPS} <<< ${SIZE} > ${DIR_MIPS_OUT}

    # comparing difference and ignore the nonsense line
    # generated on mac
    diff ${DIR_C_OUT} ${DIR_MIPS_OUT} -I "Loaded: *" > /dev/null

    if  [ $? -eq 0 ]
    then
        echo passed
    else
        echo failed
        echo "STARTING DIFFERENCE:" >> ${DIR_DIFF}
        echo "Mips included:" >> ${DIR_DIFF}
        cat ${DIR_MIPS_INCLUDE} >> ${DIR_DIFF}
        echo "C included:" >> ${DIR_DIFF}
        cat ${DIR_C_INCLUDE} >> ${DIR_DIFF}
        echo "Diff:" >> ${DIR_DIFF}
        diff ${DIR_C_OUT} ${DIR_MIPS_OUT} -I "Loaded: *" >> ${DIR_DIFF}
        echo "\n\n\n\n" >> ${DIR_DIFF}
    fi
}

for TEST_NO in {1..100}
do
    # making hidden test directory
    rm -rf ${DIR}
    mkdir ${DIR}
    
    printf "testing ${TEST_NO}..."
    SIZE=$(($RANDOM % 30 + 1))
    test ${SIZE}

    # deleting test directory
    rm -rf ${DIR}
done


