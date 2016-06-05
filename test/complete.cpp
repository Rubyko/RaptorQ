/* 
 * Copyright (c) 2016 Savchenko Yehor
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rq.h"
#include <fstream>
#include <streambuf>

int main() 
try {
	int i = 0;
	while(true){

	long F = 0;
	std::string in, out;

	std::vector<packet> packets;
	{
		std::ifstream t("/home/yegor/Documents/text.txt");
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		F = str.size();

		in = str;

		packet_producer pckt = create_encoding_packet_producer(str.begin(), str.end(), 8 * 1024 * 1024 * 8, 65500);
		
		for(int Z = 0; Z != pckt.oti.Z; Z++)
		for(int i = 0; i < pckt.blocks[Z].sys.K_prime + 2000; i++)
		{
			//	std::cout << " Z = " << Z << " i = " << i << std::endl;
				auto pk = pckt.produce(Z);
				packets.push_back(pk);
		}
	}
	// lossy channel start
	{
		std::random_shuffle(packets.begin(), packets.end());		
		packets.erase(packets.begin(), packets.begin() + 4000);
	}
	// lossy chanel end
	{
		packet_consumer consumer = create_encoding_packet_consumer(8 * 1024 * 1024 * 8, 65500);

		for(uint32_t i = 0; i != packets.size(); i++)
			consumer.consume(packets[i]);
		
		while(true)
		{
		//	std::cout << "data readiness : " << consumer.is_data_ready() << std::endl;
			if(consumer.is_data_ready())
			{
				std::string str;
				str.resize(F);
				consumer.get_data(str.begin(), str.end());
				
				//std::cout << "recovered: " << str << std::endl;
				out = str;

				std::ofstream t("/home/yegor/Documents/res.txt");
				t << str;
    				t.close();
				break;
			}
			std::this_thread::sleep_for(std::chrono::microseconds(10000));
		}
	}	
		std::cout << "decoded " << ++i << "success " << in.compare(out) << " if 0 then success " <<std::endl;
	}
	return 0;
} 
catch(std::exception e)
{
	std::cout << e.what() << std::endl;
}
