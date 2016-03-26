namespace util
{
	template <typename TVal>
	TVal FastPower(TVal val, int p) {
		if (p == 0) return 1;
		if (p == 1) return val;
		if (p % 2 == 1)
			return val * FastPower(val * val, p / 2);
		return FastPower(val * val, p / 2);
	}

	template<typename TVal> 
	TVal GCD(TVal x, TVal y) {
		while(y) {
			x %= y;
			swap(x, y);
		}
		return x;
	}
}
