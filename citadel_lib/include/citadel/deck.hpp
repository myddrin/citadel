
#ifndef CITADELLIB_DECK
#define CITADELLIB_DECK

#include <list>

namespace Citadel {

	class Building;

	class Deck
	{
	protected:
		std::list<Building*> _cards;

	public:
		Deck(){};
		virtual ~Deck();

		void shuffle();

		Building *getTop();

		void pushBack(Building *b);
		bool take(Building *b);

		//
		// list access
		//

		const std::list<Building*>& cards(void) const;
		
		const Building* search(const std::string &name) const;
		const Building* search(const Building *b) const;
	};
}

#endif // CITADELLIB_DECK
