#ifndef _INTERFACES_H
#define _INTERFACES_H

class ISwitch {
	public:
		virtual bool isActive() = 0;
};

class IUpdated1ms {
	public:
		virtual void update1ms() = 0;
};

class IUpdatedSomewhere {
	public:
		virtual void updateSomewhere() = 0;
};
#endif //_INTERFACES_H