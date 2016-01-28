#include "dtv.h"
#include <itc/error.h>
#include <QtGui/QKeyEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QPrinter>
#include <qwt_plot.h>
#include <qwt_series_data.h>
#include <qwt_legend.h>
#include <qwt_plot_renderer.h>
 
 
dtv::widget::widget(QWidget *parent)
: QwtPlot(parent)
{
	setPalette(Qt::white);
	setAutoFillBackground(true);
	setMinimumSize(256, 256);
	setFrameStyle(QFrame::Sunken | QFrame::Panel);
	setMinimumSize(256, 256);
	insertLegend(new QwtLegend(this), QwtPlot::BottomLegend);
// 	//legend()->setFont(QFont("Arial", 10));
// 	auto font=legend()->font();
// 	font.setPointSize(10);
// 	legend()->setFont(font);
}

dtv::widget::~widget()
{}


void dtv::widget::load()
{
	auto lst=load(stdin);
	for(auto graph : lst)
		graph->attach(this);
}


void dtv::widget::load(std::string const& file)
{
	FILE *fd=fopen(file.c_str(), "r");
	if(!fd)
		throw itc::syserr(file);
	
	auto lst=load(fd);
	fclose(fd);
	
	for(auto graph : lst)
		graph->attach(this);
}

void dtv::widget::memo(int sz)
{
 	auto font=legend()->font();
 	font.setPointSize(sz);
	legend()->setFont(font);
	//QFont font("Arial", sz);
}

std::list<QwtPlotCurve*> dtv::widget::load(FILE* fd)
{
	static QColor color[]={"red", "blue", "green", "magenta"};
	static int idx=0;
	std::list<QwtPlotCurve*> r;
	char buf[2048];
	std::string head;
 	QwtPlotCurve *cv=0;
	std::vector<double> x, y;
	
	while(fgets(buf, sizeof buf, fd)) {
		double _x, _y;
		if(sscanf(buf, "%lf%lf", &_x, &_y) != 2) {
			if(cv) {
				cv->setSamples(x.data(), y.data(), x.size());
				r.push_back(cv);
				cv=0;
			}
			head.append(buf);
			continue;
		}
		if(!cv) {
 			cv=new QwtPlotCurve(head.c_str());
			cv->setTitle(head.c_str());
			cv->setPen(color[(idx++)%4]);
			head.clear();
			x.clear();
			y.clear();
		}
		x.push_back(_x);
		y.push_back(_y);
	}	
	if(cv) {
		cv->setSamples(x.data(), y.data(), x.size());
		r.push_back(cv);
		cv=0;
	}
	
	return r;
}



void dtv::widget::keyPressEvent(QKeyEvent* e)
{
	e->accept();
	bool mode=e->modifiers() & Qt::ControlModifier;
	if(mode) ctrl_key_event(e);
	else plain_key_event(e);
}


void dtv::widget::plain_key_event(QKeyEvent* e)
{
	switch(e->key()) {
// 		case Qt::Key_Left: _board->show_prev(); break;
// 		case Qt::Key_Right: _board->show_next(); break;
// 		case Qt::Key_Insert: _board->mark_checked(); break;
// 		case Qt::Key_Delete: _board->mark_unchecked(); break;
// 		case Qt::Key_PageUp: _board->show_prev_checked(); break;
// 		case Qt::Key_PageDown: _board->show_next_checked(); break;
// 		case Qt::Key_Home: _board->show_first(); break;
// 		case Qt::Key_End: _board->show_last(); break;
// 		case Qt::Key_F1: _board->scale(SCALE_INDIVIDUAL); break;
// 		case Qt::Key_F2: _board->scale(SCALE_SHARED); break;
// 		case Qt::Key_F3: _board->scale(SCALE_ASME); break;
		default: e->ignore(); break;
	}
}


void dtv::widget::ctrl_key_event(QKeyEvent* e)
{
	switch(e->key()) {
		case Qt::Key_Q: emit(quit()); update(); break;
		case Qt::Key_X: rcode(1); emit(quit()); update(); break;
		case Qt::Key_C: rcode(2); emit(quit()); update(); break;
		case Qt::Key_S: save(); break;
		case Qt::Key_P: print(); break;
// 		case Qt::Key_Left: _board->show_prev_checked(); break;
// 		case Qt::Key_Right: _board->show_next_checked(); break;
// 		case Qt::Key_Insert: _board->mark_checked_all(); break;
// 		case Qt::Key_Delete: _board->mark_unchecked_all(); break;
// 		case Qt::Key_M: _board->toggle_checked(); break;
// 		case Qt::Key_P: _board->print(); break;
// 		case Qt::Key_1: _board->scale(SCALE_INDIVIDUAL); break;
// 		case Qt::Key_2: _board->scale(SCALE_SHARED); break;
// 		case Qt::Key_3: _board->scale(SCALE_ASME); break;
		default: e->ignore(); break;
	}
}


void dtv::widget::save()
{
	auto file=QFileDialog::getSaveFileName(this);
	QwtPlotRenderer r(this);
	r.renderDocument(this, file, QSizeF(300,200));
}


void dtv::widget::print()
{
	QPrinter printer;
	QwtPlotRenderer r(this);
	r.renderTo(this, printer);
}



