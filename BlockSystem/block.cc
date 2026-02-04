#include "block.h"
#include <QVariant>
QString Block::typeName()const{
	return "Block";
}
void Block::deleteObj()const{
	delete this;
}

// class DBBloc : Block{
//
// };
// class Page : Block{
//
// };
