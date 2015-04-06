#ifndef __RLG_H__
#define __RLG_H__

#include <math.h>
#include <sstream>

class RLG {
	private:
		static unsigned int counter;

		char indexToChar(unsigned int idx) {
			if (idx <= 25) {
				return (char)(97 + idx);
			} else {
				return (char)(65 + (idx % 26));
			}
		}
	public:
		std::string operator()() {
			std::stringstream ss;
			unsigned int label_length = 1;

			while (pow(52, label_length) <= counter) {
				label_length++;
			}

			unsigned int copy = counter;
			unsigned int lol = label_length;
			label_length--;
			for (unsigned int i = 0; i < lol; i++) {
				unsigned int div_result = copy / (unsigned int)(pow(52, label_length));
				unsigned int mod_result = copy % (unsigned int)(pow(52, label_length));

				ss << indexToChar(div_result);
				copy = mod_result;

				label_length--;
			}

			RLG::counter++;
			return ss.str() + '$';
		}
};

unsigned int RLG::counter = 0;

static RLG LABEL_GEN;

#endif