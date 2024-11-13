function sumNums(nums) {
    if (nums) {
        return nums.reduce((acc, n) => acc + n, 0);
    } else {
        return 0;
    }
}

function main() {
    const args = process.argv.slice(2).map((arg) => Number(arg));
    console.log(sumNums(args));
}

main();
