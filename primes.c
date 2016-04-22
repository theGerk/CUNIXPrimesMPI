#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILE_NAME "primes.dat"

typedef struct {
	unsigned long long* arr;
	size_t FULL_LENGTH;
	size_t used;
} arrLen;


char isPrime(unsigned long long value, unsigned long long* primes, unsigned long long length);
static inline unsigned long long getChunkEnd(unsigned long long curMin);	//min is inclusive and end (output) is excusive
void getPrimes(unsigned long long max, arrLen* primes);
off_t fsize(const char *filename);
int main(int argv, char** argc)
{
	unsigned long long MAX;
	scanf("%lu", &MAX);
	arrLen primes;
	getPrimes(MAX, &primes);
	printf("There are %d primes less then %ld\n", primes.used, MAX);
	return 0;
}

char isPrime(unsigned long long value, unsigned long long* primes, unsigned long long length)
{
	unsigned long long max = (unsigned long long)(sqrt(value));
	unsigned long long c;
//	printf("max: %d\tvalue: %d\tlength: %d\n", max, value, length);
	for (unsigned long long i = 0; (c = primes[i]) <= max; i++)
	{
//		printf("value: %d\tc: %d\n", value, c);
		if (value % c == 0)
		{
			return 0;
		}
	}
	return 1;
}

static inline unsigned long long getChunkEnd(unsigned long long curMin)
{
	return curMin * curMin;
}

/*for x > 598, (x/log x)(1 + 1.2762/log x)*/
void getPrimes(unsigned long long max, arrLen* primes)
{
	primes->arr = malloc(sizeof(long long) * (primes->FULL_LENGTH = max));
	//primes->arr = malloc(sizeof(max) * (primes->FULL_LENGTH = (unsigned long long)((max <= 598) ? 110 : (max / log(max)) * (1 + 1.2762 / log(max)))));
	printf("You have %d spaces.\n", primes->FULL_LENGTH);

	off_t fileSize = fsize(FILE_NAME);
	int myFile;
	if(fileSize >= sizeof(long long)) {
		myFile = open(FILE_NAME, O_RDONLY);
        read(myFile, primes->arr, fileSize);
        close(myFile);
        myFile = open(FILE_NAME, O_WRONLY | O_APPEND);
	} else {
		primes->arr[0] = 3;
		myFile = open(FILE_NAME, O_WRONLY);
		write(myFile, primes->arr, sizeof(long long));
	}

//int numberOfBytesTransferredOrNegativeOneIfError = read(thefile,whereYouWantToWriteTheDataInTheFileTo,theAmountOfDataInBytesToReadFromTheFile);
//int numberOfBytesWrittenOrNEgitiveOneIfError = write(theFile,FromWhere,BytesToWrite)
	size_t primeIndex = 1;
	unsigned long long current = 5;
	unsigned long long chunkEnd;
	while (current < max)
	{
		chunkEnd = getChunkEnd(current);
		//chunkEnd = current + 100000;
		if(chunkEnd > max)
			chunkEnd = max;
		//get next chunk of numbers (MASTER ONLY)


		//divide up for processors (DO IN HALFS)

		//do work (ALL THE PROCESSORS)
		while (current < chunkEnd) // change when being split between processors.
		{
			if (isPrime(current, primes->arr, primeIndex))
			{
				primes->arr[primeIndex++] = current;
			}
			current+=2;
		}

		printf("Found prime number %lld: %lld\tChunk: %lld\n", primeIndex + 1, primes->arr[primeIndex], current);
		//get push in primes in order (DO IN HALFS, use merge sort?
	}

	primes->used = primeIndex + 1;

	close(myFile);
}

off_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}
