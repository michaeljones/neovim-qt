#ifndef NEOVIM_QT_SHELL
#define NEOVIM_QT_SHELL

#include <QWidget>
#include <QVariantList>
#include <QFont>
#include <QBackingStore>
#include "neovimconnector.h"

namespace NeovimQt {

class Shell: public QWidget
{
	Q_OBJECT
public:
	Shell(NeovimConnector *nvim, QWidget *parent=0);
	~Shell();
	QSize sizeIncrement() const;
	QSize sizeHint() const;
	static QColor color(qint64 color, const QColor& fallback=QColor());

public slots:
	void handleNeovimNotification(const QByteArray &name, const QVariantList& args);

protected slots:
	void neovimIsReady();
	void neovimError(NeovimConnector::NeovimError);
	void neovimExited(int);

protected:
	quint64 neovimWidth() const;
	quint64 neovimHeight() const;
	quint64 neovimRowHeight() const;
	quint64 neovimCellWidth() const;
	QSize neovimSize() const;
	QPoint neovimCursorTopLeft();
	QSize neovimCharSize() const;
	void setCursor(quint64 col, quint64 row);
	void setupPainter(QPainter&);

	virtual void resizeEvent(QResizeEvent *ev) Q_DECL_OVERRIDE;
	virtual void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;
	virtual void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
	virtual void changeEvent(QEvent *ev) Q_DECL_OVERRIDE;
	virtual void closeEvent(QCloseEvent *ev) Q_DECL_OVERRIDE;

	virtual void handleResize(uint64_t cols, uint64_t rows);
	virtual void handlePut(const QVariantList& args, QPainter&);
	virtual void handleHighlightSet(const QVariantMap& args, QPainter& painter);
	virtual void handleRedraw(const QByteArray& name, const QVariantList& args, QPainter& painter);
	virtual void handleScroll(const QVariantList& args, QPainter& painter);
	virtual void handleNormalMode(QPainter& painter);
	virtual void handleInsertMode(QPainter& painter);
	virtual void handleSetScrollRegion(const QVariantList& opargs);
private:
	bool m_attached;
	NeovimConnector *m_nvim;
	uint64_t m_rows, m_cols;
	QRect m_scroll_region;

	QFont m_font;
	QFontMetrics *m_fm;

	QImage m_image;

	// shell bg/fg - updated through redraw:foreground/background
	QColor m_foreground, m_background;
	// highlight fg/bg - from redraw:highlightset - by default we
	// use the values from above
	QColor m_hg_foreground, m_hg_background;
	QColor m_cursor_color;

	QPoint m_cursor_pos;
	bool m_cursor;
	bool m_insertMode;
};

} // Namespace
#endif
