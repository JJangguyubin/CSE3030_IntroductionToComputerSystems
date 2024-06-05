#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

cache_t* init_cache(int b, int E, int s) {
  // TODO: Complete this function (some code is given).
	cache_t *cache = malloc(sizeof(cache_t));

	cache->b = b; // block offset의 bit 수.
	cache->B = 1 << b; // B = 2^b.
	cache->E = E; // 각 Set마다 가지고 있는 Line의 수.
	cache->s = s; // set index의 bit 수.
	cache->S = 1 << s; // S = 2^s. 전체 Set의 총 갯수.
	cache->sets = malloc(sizeof(cache_set_t) * cache->S); // 전체 Set의 갯수만큼 동적으로 할당.
	for (int i = 0; i < cache->S; i++) // 각 Set마다 E개의 Line 동적으로 할당. 그리고 초기화..!!
	{
		cache->sets[i] = malloc(sizeof(cache_line_t) * E); // 각 Set마다 E개의 Line을 동적으로 할당.
		for (int j = 0; j < E; j++)
		{
			cache->sets[i][j].valid_flag = 0;
			cache->sets[i][j].tag = 0;
			cache->sets[i][j].age = 0;
		}
	}

	return cache;
}

// addr는 10진수 정수의 형태라고 생각하면 편하다. 이를 2진수로 변환해주는 작업을 해줘야 한다..!!
int access_memory(cache_t* cache, addr_t addr) {
	// TODO: Complete this function.

	// 일단, 접근된 메모리 주소가 Cache Hit인지 Cache Miss인지 확인하기 전에,
	// 모든 Set의 모든 유효한 Line에 대하여, age를 1씩 증가시키는 작업을 진행해야합니다.
	for (int i = 0; i < cache->S; i++)
	{
		for (int j = 0; j < cache->E; j++)
		{
			if (cache->sets[i][j].valid_flag == 1)
			{
				cache->sets[i][j].age += 1;
			}
		}
	} 

	// 혹시 모를 위험을 대비해서, 변수의 타입을 addr_t로 한다.
	addr_t accessed_memory_block_offset = 0; // 접근된 메모리 주소의 block offset.
	addr_t accessed_memory_set_index = 0; // 접근된 메모리 주소의 set index.
	addr_t accessed_memory_tag_bits = 0; // 접근된 메모리 주소의 tag bits.
	
	// 혹시 모를 위험을 대비해서, 변수의 타입을 addr_t로 한다.
	addr_t x = 1;
	

	// 접근된 메모리주소의 block offset 구하기.
	for (int i = 0; i < cache->b; i++)
	{	
		accessed_memory_block_offset += x * (addr % 2);
		addr /= 2;
		x *= 2;	
	}

	x = 1;

	// 접근된 메모리주소의 set index 구하기.
	for (int i = 0; i < cache->s; i++)
	{
		accessed_memory_set_index += x * (addr % 2);
		addr /= 2;
		x *= 2;
	}

	x = 1;
	
	// 접근된 메모리 주소의 tag bits 구하기.
 	while (addr != 0)
	{
		accessed_memory_tag_bits += x * (addr % 2);
		addr /= 2;
		x *= 2;
	}

	// 접근된 메모리 주소의 set index를 구했으므로, set index 값에 해당하는 Set으로 가서, 
	// 해당 Set의 모든 라인을 탐색하면서,
	// valid bit가 1이면서, tag bits가 일치하는지 확인한다.
	// set index에 해당하는 해당 Set의 모든 Line을 탐색한다.
	for (int i = 0; i < cache->E; i++)
	{	
		// valid bit가 1이면서,
		if (cache->sets[accessed_memory_set_index][i].valid_flag == 1)
		{
			// 해당 line의 tag bits와 접근된 메모리 주소의 tag bits가 같다면,
			if (cache->sets[accessed_memory_set_index][i].tag == accessed_memory_tag_bits)
			{	
				// Cache Hit이다.
				// 그리고, 해당 Line의 age를 0으로 초기화(다시 설정) 해준다.
				cache->sets[accessed_memory_set_index][i].age = 0;

				return 1;
			}
		}
	}

	// 여기서부터 Cache Miss가 일어난 이후, 접근된 메모리 주소를 포함하는 블록을 Cache의 해당 Set에 업데이트하는 과정을 진행합니다.

	int done = 0; // Cache Miss가 일어난 이후로, 접근된  메모리 주소를 포함하는 블록을 Cache의 해당 Set에 정상적으로 등록했는지 여부를 알려주는 변수입니다.

	// 먼저, 해당 Set에 Empty Line이 있을 경우 그 Empty Line으로 접근된 메모리 주소를 포함한 블록을 등록(업데이트)한다.
	// 모든 Line을 탐색하면서,
	for (int i = 0; i < cache->E; i++)
	{
		// Empty Line이 있는지 확인한다.
		// 만약 Empty Line이 있다면, 그 Line에 접근된 메모리 주소를 포함한 블록 정보를 등록한다.
		// 만약 해당 Line이 Empty Line이라면,
		if (cache->sets[accessed_memory_set_index][i].valid_flag == 0)
		{
			// 해당 Line의 valid bit를 1로 설정해주고,
			cache->sets[accessed_memory_set_index][i].valid_flag = 1;
			// 해당 Line의 tag bits를 접근된 메모리 주소의 tag bits로 업데이트 한다.
			cache->sets[accessed_memory_set_index][i].tag = accessed_memory_tag_bits;
			// 그리고 해당 Line에 새롭게 들어온 블록이기 때문에, age를 0으로 초기화해준다.
			cache->sets[accessed_memory_set_index][i].age = 0;
			
			// 그리고, 'done = 1;'을 함으로써, 접근된 메모리 주소를 포함하는 블록을 성공적으로 Cache의 해당 Set에 업데이트 했음을 알린다.
			done = 1;
			break;
		}
	}
	
	// 'done == 0'인 상황이라는 것은 해당 Set에 Empty 라인이 없다는 의미이므로,
	// 우리가 오늘 배운 Least Recently Used 즉, LRU 교체 정책을 활용하여 특정 라인의 블록(?)을 추방하고, 해당 라인에 접근된 메모리 주소를 포함하는 블록을 등록(?)해야 한다.
	if (done == 0)
	{
		int age_max_idx = 0; // 가장 age가 많은 Line의 idx.
		int age_max = -1; // 가장 age가 많은 Line의 age.

		// 해당 Set에 모든 Line을 순회하면서, 가장 덜 최근에 사용된 Line을 찾는다.
		for (int i = 0; i < cache->E; i++)
		{
			if (age_max < cache->sets[accessed_memory_set_index][i].age)
			{
				age_max = cache->sets[accessed_memory_set_index][i].age;
				age_max_idx = i;
			}			
		}
		
		// 가장 덜 최근에 사용된 Line을 찾은 후에, 해당 라인에 접근된 메모리 주소를 포함한 블록을 등록하고, valid bit를 1로 바꾸고, tag bits를 업데이트 하고, age를 0으로 초기화한다.
		cache->sets[accessed_memory_set_index][age_max_idx].valid_flag = 1;
		cache->sets[accessed_memory_set_index][age_max_idx].tag = accessed_memory_tag_bits;
		cache->sets[accessed_memory_set_index][age_max_idx].age = 0;
	}

	return 0;
}

void free_cache(cache_t* cache) {
  // TODO: Complete this function (some code is given).
	
	for (int i = 0; i < cache->S; i++)
	{
		free(cache->sets[i]);
	}
	free(cache->sets);
	
	free(cache);
}
