#ifndef DTV_H
#define DTV_H
#include <QtGui/QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
namespace dtv {
 
class widget : public QwtPlot
{
Q_OBJECT
public:
	widget(QWidget *parent =0);
	~widget();
	void load(std::string const&);
	void load();
	int rcode() const { return _rcode; }
	int rcode(int x) { return _rcode=x; }
	
	void memo(int);

signals:
	void quit();

protected:
	void keyPressEvent(QKeyEvent*);
	
private:
	int _rcode=0;
	void plain_key_event(QKeyEvent*);
	void ctrl_key_event(QKeyEvent*);
	std::list<QwtPlotCurve*> load(FILE*);
	void save();
	void print();
};


}//dtv
#endif

