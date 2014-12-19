#ifndef _FS_SCROLL_WIDGET_H_
#define _FS_SCROLL_WIDGET_H_

#include "FsMacros.h"
#include "FsUiWidget.h"

NS_FS_BEGIN

class VelocityTracker;
class Scroller;

class ScrollWidget:public UiWidget
{
	public:
		FS_CLASS_DECLARE(ScrollWidget);

	public:
		/* inherit Entity */
		void update(float dt) FS_OVERRIDE;

		bool touchBegin(float x,float y) FS_OVERRIDE;
		bool touchMove(float x,float y) FS_OVERRIDE;
		bool touchEnd(float x,float y) FS_OVERRIDE;


		/* inherit UiWidget */
		void childSizeChanged(UiWidget* widget) FS_OVERRIDE;
		void childAnchorChanged(UiWidget* widge) FS_OVERRIDE;
		void childTransformChanged(UiWidget* widget) FS_OVERRIDE;


		using UiWidget::setSize;
		void setSize(const Vector2& size) FS_OVERRIDE;

		using UiWidget::setAnchor;
		void setAnchor(const Vector2& size) FS_OVERRIDE;


	public:
		virtual void scrollChange(float x,float y);
		virtual void beginDrag();
		virtual void endDrag();


	public:
		void setMargin(float l,float r,float t,float b);
		void getMargin(float* l,float* r,float* t,float* b);

		void setMarginLeft(float v);
		float getMarginLeft() const;

		void setMarginRight(float v);
		float getMarginRight() const;

		void setMarginTop(float v);
		float getMarginTop() const ;

		void setMarginBottom(float v);
		float getMarginBottom() const;

		void setScrollMode(E_ScrollDirection mode);
		E_ScrollDirection getScrollMode();

		void setEdgeBounceEnabled(bool enabled);
		bool getEdgeBounceEnabled();


	public:
		void setContentAlign(E_AlignH h,E_AlignV v);


	public: /* scroll support */
		void scrollBy(float x,float y);
		void scrollTo(float x,float y);

		void setScrollPercent(float x,float y);
		void setScrollPercentX(float x);
		void setScrollPercentY(float y);

		void getScrollPercent(float* x,float* y);
		float getScrollPercentX();
		float getScrollPercentY();

		void layout();

	protected:

		ScrollWidget();
		virtual ~ScrollWidget();

		void adjustScrollArea();

		void startScrollX(float cur,float min,float max,float delta);
		void startScrollY(float cur,float min,float max,float delta);

		void updateScroll(float dt);
		void edgeCheckHandle();

		void setContentSize(float width,float height);




	protected:
		float m_scrollX,m_scrollY;

		float m_touchTap;

		float m_contentWidth,m_contentHeight;

		float m_marginLeft,m_marginRight,m_marginTop,m_marginBottom;

		float m_scrollMinX,m_scrollMaxX;
		float m_scrollMinY,m_scrollMaxY;


		/* attr cache */
		float m_edgeLeft,m_edgeRight,m_edgeTop,m_edgeBottom;
		float m_edgeBLeft,m_edgeBRight,m_edgeBTop,m_edgeBBottom;


		/* drag info */

		bool m_cancelDrag,m_isDraged;
		float m_lastMotionPosX, m_lastMotionPosY; 


		bool m_press;
		int m_scrollState;

		Scroller* m_scrollerX;
		Scroller* m_scrollerY;

		E_ScrollDirection m_scrollMode;

		E_AlignH m_alignh;
		E_AlignV m_alignv;

		VelocityTracker* m_velocityTracker;

		bool m_edgeBounceEnabled;

};

NS_FS_END

#endif /*_FS_SCROLL_WIDGET_H_*/


