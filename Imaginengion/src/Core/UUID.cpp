#include "impch.h"
#include "UUID.h"

namespace IM {

	namespace {
        std::array<uint64_t, std::mt19937_64::state_size> s_SeedData{
            []() {
                // Fill the seed data array with random values from std::random_device
                std::array<uint64_t, std::mt19937_64::state_size> seedData{};
                std::random_device s_RandomDevice;
                for (size_t i = 0; i < seedData.size(); ++i) {
                    seedData[i] = s_RandomDevice();
                }
                return seedData;
            }()
        };

        // Initialize the seed sequence with the seed data
        std::seed_seq s_Seq(std::begin(s_SeedData), std::end(s_SeedData));

        // Initialize the random number generator with the seed sequence
        std::mt19937_64 s_Engine(s_Seq);
        std::uniform_int_distribution<uint64_t> s_UniformDistribution;
	}

	UUID::UUID()
		: _UUID(s_UniformDistribution(s_Engine)){

	}
	UUID::UUID(uint64_t uuid)
		: _UUID(uuid){
	}
}