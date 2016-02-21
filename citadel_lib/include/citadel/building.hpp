
#ifndef CITADELLIB_BUILDING
#define CITADELLIB_BUILDING

#include <string>

namespace Citadel {

	class Building
	{
	public:
		enum Type {
			BUILD_TRADE = 0,
			BUILD_MILITARY,
			BUILD_RELIGIOUS,
			BUILD_NOBLE,
			BUILD_WONDER,

			BUILD_N,
		};
	protected:
		const std::string _name;
		const std::string _description;
		const unsigned int _price;
		const enum Type _type;

		unsigned int _value;
		int _builtAtTurn; // negative if not built yet

	public:
		Building(const std::string &name, unsigned int price, enum Type t,
			const std::string &desc = "");

		//
		// accessors
		//
		std::string name() const;
		
		std::string description() const;
		
		unsigned int price() const;
		
		unsigned int value() const;
		void setValue(unsigned int v);
		
		enum Type type() const;
		
		int builtAtTurn() const;

		void setBuiltAtTurn(int turn);
	};
}

#endif // CITADELLIB_BUILDING