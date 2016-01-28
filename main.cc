#include <QtGui/QApplication>
#include "dtv.h"
#include <unistd.h>
#include <iostream>

 
int main(int argc, char *argv[])
{
try {
	QApplication app(argc, argv);
	dtv::widget *x=new dtv::widget();
	QObject::connect(x, SIGNAL(quit()), qApp, SLOT(quit()));

	
	int opt;
	while((opt=getopt(argc,argv,"x:y:L:h")) != -1)
	switch(opt) {
		case 'h':
		case 'x': x->setAxisTitle(2, optarg); break;
		case 'y': x->setAxisTitle(0, optarg); break;
		case 'L': x->memo(atol(optarg)); break;
		default : return 1;
	}
	
	if(optind >= argc)
		x->load();
	for(int n=optind; n < argc; ++n) {
		if(argv[n] == std::string("-"))
			x->load();
		else
			x->load(argv[n]);
	}


	x->show();
	app.exec();
	return x->rcode();

} catch(std::exception const& x) {
	std::cerr<<x.what()<<std::endl;
	return 1;
}
}
