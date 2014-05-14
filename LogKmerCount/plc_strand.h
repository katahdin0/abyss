/**
 * A minifloat like datatype for probablistic log counts (PLC) of elements
 * Contains unique functionalities for minimizing collision
 * Copyright 2014 bcgsc
 */

static const uint8_t numericBitMask = 0x1F;
static const unsigned mantissa = 1;

class plc {
public:
	plc()
	{
		m_val = 0;
	}

	void operator++()
	{
		//extract numeric section of plc
		uint8_t maskedVal = m_val & numericBitMask;

		//check if at max value
		if (maskedVal > 0x1F) {
			return;
		}

		if (maskedVal <= mantissa) {
			++m_val;
		} else {
			//this shifts the first bit off and creates the value
			//need to get the correct transition probability
			unsigned shiftVal = 1 << ((maskedVal >> mantissa) - 1);
			if (rand() % shiftVal == 0) {
				++maskedVal;
			}
		}
	}

	float toFloat()
	{
		//extract numeric section of plc
		uint8_t maskedVal = m_val & numericBitMask;

		if (maskedVal <= mantissa)
			return float(maskedVal);
		//the following needs to modified if mantissa is changed
		return float((maskedVal & 0x01) | 0x02)
				* pow(2.0, (maskedVal >> mantissa) - mantissa);
	}

	/*
	 * return raw value of byte use to store value
	 */
	uint8_t rawValue()
	{
		return m_val;
	}

private:
	uint8_t m_val;
};
