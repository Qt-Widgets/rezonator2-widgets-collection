#ifndef SCHEMA_LAYOUT_LENS_H
#define SCHEMA_LAYOUT_LENS_H

#include "SchemaLayout.h"

#include <QtMath>

using namespace ElementLayoutProps;

//------------------------------------------------------------------------------
namespace CurvedElementLayout {
    enum CurvedForm {
        FormUnknown,
        ConvexLens,         //      ()
        ConcaveLens,        //      )(
        ConvexConcaveLens,  //      ((
        ConcaveConvexLens,  //      ))
        PlanoConvexMirror,  //      |)
        PlanoConvexLens,    //      |)
        PlanoConcaveMirror, //      |(
        PlanoConcaveLens,   //      |(
        ConvexPlanoMirror,  //      (|
        ConvexPlanoLens,    //      (|
        ConcavePlanoMirror, //      )|
        ConcavePlanoLens    //      )|
    };

    DECLARE_ELEMENT_LAYOUT_BEGIN
        QBrush brush;
        CurvedForm paintMode = FormUnknown;
        void paintCornerMark(QPainter *painter, const QString& mark) const;
    DECLARE_ELEMENT_LAYOUT_END

    ELEMENT_LAYOUT_INIT {
    }

    ELEMENT_LAYOUT_PAINT {
        painter->setBrush(brush.style() == Qt::NoBrush ? getGlassBrush() : brush);
        painter->setPen(getGlassPen());

        const qreal ROC = 100;
        const qreal sagitta = ROC - qSqrt(Sqr(ROC) - Sqr(HH));
        const qreal startAngle = qRadiansToDegrees(qAsin(HH / ROC));
        const qreal sweepAngle = 2*startAngle;

        QPainterPath path;
        QRectF rightSurface;
        QRectF leftSurface;

        switch (paintMode) {
        case FormUnknown:
            break;

        case ConvexLens: // ()
            rightSurface = QRectF(HW - 2*ROC, -ROC, 2*ROC, 2*ROC);
            leftSurface = QRectF(-HW, -ROC, 2*ROC, 2*ROC);
            path.moveTo(HW - sagitta, HH);
            path.arcTo(rightSurface, 360-startAngle, sweepAngle);
            path.lineTo(-HW + sagitta, -HH);
            path.arcTo(leftSurface, 180-startAngle, sweepAngle);
            break;

        case ConcaveLens: // )(
            rightSurface = QRectF(HW - sagitta, -ROC, 2*ROC, 2*ROC);
            leftSurface = QRectF(-HW + sagitta - 2*ROC, -ROC, 2*ROC, 2*ROC);
            path.moveTo(HW, -HH);
            path.arcTo(rightSurface, 180-startAngle, sweepAngle);
            path.lineTo(-HW, HH);
            path.arcTo(leftSurface, 360-startAngle, sweepAngle);
            break;

        case ConvexConcaveLens: // ((
            leftSurface = QRectF(-HW, -ROC, 2*ROC, 2*ROC);
            rightSurface = QRectF(HW - sagitta, -ROC, 2*ROC, 2*ROC);
            path.moveTo(-HW + sagitta, -HH);
            path.arcTo(leftSurface, 180-startAngle, sweepAngle);
            path.lineTo(HW, HH);
            path.arcTo(rightSurface, 180+startAngle, -sweepAngle);
            break;

        case ConcaveConvexLens: // ))
            leftSurface = QRectF(-HW + sagitta - 2*ROC, -ROC, 2*ROC, 2*ROC);
            rightSurface = QRectF(HW - 2*ROC, -ROC, 2*ROC, 2*ROC);
            path.moveTo(-HW, -HH);
            path.arcTo(leftSurface, startAngle, -sweepAngle);
            path.lineTo(HW - sagitta, HH);
            path.arcTo(rightSurface, -startAngle, sweepAngle);
            break;

        case PlanoConvexMirror:
            painter->setBrush(getMirrorBrush());
            rightSurface = QRectF(HW - 2*ROC, -ROC, 2*ROC, 2*ROC);
            path.moveTo(HW - sagitta, HH);
            path.arcTo(rightSurface, 360-startAngle, sweepAngle);
            path.lineTo(-HW, -HH);
            path.lineTo(-HW, HH);
            break;

        case PlanoConvexLens:
            rightSurface = QRectF(HW - 2*ROC, -ROC, 2*ROC, 2*ROC);
            path.moveTo(HW - sagitta, HH);
            path.arcTo(rightSurface, 360-startAngle, sweepAngle);
            path.lineTo(-HW, -HH);
            path.lineTo(-HW, HH);
            break;

        case PlanoConcaveMirror:
            painter->setBrush(getMirrorBrush());
            rightSurface = QRectF(HW - sagitta, -ROC, 2*ROC, 2*ROC);
            path.moveTo(HW, -HH);
            path.arcTo(rightSurface, 180-startAngle, sweepAngle);
            path.lineTo(-HW, HH);
            path.lineTo(-HW, -HH);
            break;

        case PlanoConcaveLens:
            rightSurface = QRectF(HW - sagitta, -ROC, 2*ROC, 2*ROC);
            path.moveTo(HW, -HH);
            path.arcTo(rightSurface, 180-startAngle, sweepAngle);
            path.lineTo(-HW, HH);
            path.lineTo(-HW, -HH);
            break;

        case ConcavePlanoMirror:
            painter->setBrush(getMirrorBrush());
            leftSurface = QRectF(-HW + sagitta - 2*ROC, -ROC, 2*ROC, 2*ROC);
            path.moveTo(-HW, HH);
            path.arcTo(leftSurface, 360-startAngle, sweepAngle);
            path.lineTo(HW, -HH);
            path.lineTo(HW, HH);
            break;

        case ConcavePlanoLens:
            leftSurface = QRectF(-HW + sagitta - 2*ROC, -ROC, 2*ROC, 2*ROC);
            path.moveTo(-HW, HH);
            path.arcTo(leftSurface, 360-startAngle, sweepAngle);
            path.lineTo(HW, -HH);
            path.lineTo(HW, HH);
            break;

        case ConvexPlanoMirror:
            painter->setBrush(getMirrorBrush());
            leftSurface = QRectF(-HW, -ROC, 2*ROC, 2*ROC);
            path.moveTo(-HW + sagitta, -HH);
            path.arcTo(leftSurface, 180-startAngle, sweepAngle);
            path.lineTo(HW, HH);
            path.lineTo(HW, -HH);
            break;

        case ConvexPlanoLens:
            leftSurface = QRectF(-HW, -ROC, 2*ROC, 2*ROC);
            path.moveTo(-HW + sagitta, -HH);
            path.arcTo(leftSurface, 180-startAngle, sweepAngle);
            path.lineTo(HW, HH);
            path.lineTo(HW, -HH);
            break;
        }
        path.closeSubpath();

        slopePainter(painter);
        painter->drawPath(path);

        switch (paintMode) {
        case PlanoConvexMirror:
            painter->setPen(getMirrorPen());
            painter->drawArc(rightSurface, int(16*(360-startAngle)), int(16*sweepAngle));
            break;

        case PlanoConcaveMirror:
            painter->setPen(getMirrorPen());
            painter->drawArc(rightSurface, int(16*(180-startAngle)), int(16*sweepAngle));
            break;

        case ConcavePlanoMirror:
            painter->setPen(getMirrorPen());
            painter->drawArc(leftSurface, int(16*(360-startAngle)), int(16*sweepAngle));
            break;

        case ConvexPlanoMirror:
            painter->setPen(getMirrorPen());
            painter->drawArc(leftSurface, int(16*(180-startAngle)), int(16*sweepAngle));
            break;

        default: break;
        }
    }

    void Layout::paintCornerMark(QPainter *painter, const QString& mark) const {
        auto p = boundingRect().bottomRight();
        p.setX(p.x()-qreal(HW)/2.0);
        painter->setFont(getMarkTSFont());
        painter->drawText(p, mark);
    }
}

//------------------------------------------------------------------------------
namespace ElemCurveMirrorLayout {
    DECLARE_ELEMENT_LAYOUT_BEGIN
        QSharedPointer<CurvedElementLayout::Layout> layout;
    DECLARE_ELEMENT_LAYOUT_END

    ELEMENT_LAYOUT_INIT {
        HW = 10; HH = 40;
        ElemCurveMirror *mirror = dynamic_cast<ElemCurveMirror*>(_element);
        if (!mirror || !_element->owner()) return;
        layout.reset(new CurvedElementLayout::Layout(nullptr));
        layout->setHalfSize(HW, HH);
        layout->setSlope(mirror->alpha());
        auto pos = _element->owner()->position(_element);
        if (pos == ElementOwner::PositionAtLeft)
            layout->paintMode = mirror->radius() > 0
                ? CurvedElementLayout::PlanoConcaveMirror
                : CurvedElementLayout::PlanoConvexMirror;
        else if (pos == ElementOwner::PositionAtRight)
            layout->paintMode = mirror->radius() > 0
                ? CurvedElementLayout::ConcavePlanoMirror
                : CurvedElementLayout::ConvexPlanoMirror;
        else
            layout->paintMode = mirror->radius() > 0
                ? CurvedElementLayout::ConvexLens
                : CurvedElementLayout::ConcaveLens;
        layout->init();
    }

    ELEMENT_LAYOUT_PAINT {
        if (layout) layout->paint(painter, nullptr, nullptr);
    }
}

//------------------------------------------------------------------------------
namespace ElemThinLensLayout {
    DECLARE_ELEMENT_LAYOUT_BEGIN
        QSharedPointer<CurvedElementLayout::Layout> layout;
    DECLARE_ELEMENT_LAYOUT_END

    ELEMENT_LAYOUT_INIT {
        HW = 10; HH = 40;
        auto lens = dynamic_cast<ElemThinLens*>(_element);
        if (!lens) return;
        layout.reset(new CurvedElementLayout::Layout(nullptr));
        layout->setHalfSize(HW, HH);
        layout->setSlope(lens->alpha());
        layout->paintMode = lens->focus() > 0
                       ? CurvedElementLayout::ConvexLens
                       : CurvedElementLayout::ConcaveLens;
        layout->init();
    }

    ELEMENT_LAYOUT_PAINT {
        if (layout) layout->paint(painter, nullptr, nullptr);
    }
}

//------------------------------------------------------------------------------
namespace ElemCylinderLensTLayout {
    DECLARE_ELEMENT_LAYOUT_BEGIN
        QSharedPointer<CurvedElementLayout::Layout> layout;
    DECLARE_ELEMENT_LAYOUT_END

    ELEMENT_LAYOUT_INIT {
        HW = 10; HH = 40;
        auto lens = dynamic_cast<ElemCylinderLensT*>(_element);
        if (!lens) return;
        layout.reset(new CurvedElementLayout::Layout(nullptr));
        layout->setHalfSize(HW, HH);
        layout->setSlope(lens->alpha());
        layout->paintMode = lens->focus() > 0
                           ? CurvedElementLayout::ConvexLens
                           : CurvedElementLayout::ConcaveLens;
        layout->init();
    }

    ELEMENT_LAYOUT_PAINT {
        if (layout) {
            layout->paint(painter, nullptr, nullptr);
            layout->paintCornerMark(painter, QStringLiteral("T"));
        }
    }
}

//------------------------------------------------------------------------------
namespace ElemCylinderLensSLayout {
    DECLARE_ELEMENT_LAYOUT_BEGIN
        QSharedPointer<CurvedElementLayout::Layout> layout;
    DECLARE_ELEMENT_LAYOUT_END

    ELEMENT_LAYOUT_INIT {
        HW = 10; HH = 40;
        auto lens = dynamic_cast<ElemCylinderLensS*>(_element);
        if (!lens) return;
        layout.reset(new CurvedElementLayout::Layout(nullptr));
        layout->setHalfSize(HW, HH);
        layout->setSlope(lens->alpha());
        layout->paintMode = lens->focus() > 0
                ? CurvedElementLayout::ConvexLens
                : CurvedElementLayout::ConcaveLens;
        layout->init();
    }

    ELEMENT_LAYOUT_PAINT {
        if (layout) {
            layout->paint(painter, nullptr, nullptr);
            layout->paintCornerMark(painter, QStringLiteral("S"));
        }
    }
}

//------------------------------------------------------------------------------
namespace ElemThickLensLayout {
    DECLARE_ELEMENT_LAYOUT_BEGIN
        QSharedPointer<CurvedElementLayout::Layout> layout;
    DECLARE_ELEMENT_LAYOUT_END

    ELEMENT_LAYOUT_INIT {
        HW = _element->layoutOptions.drawNarrow ? 10 : 25;
        HH = 40;
        auto lens = dynamic_cast<ElemThickLens*>(_element);
        if (!lens) return;
        layout.reset(new CurvedElementLayout::Layout(nullptr));
        layout->setHalfSize(HW, HH);
        auto R1 = lens->radius1();
        auto R2 = lens->radius2();
        if (R1 < 0 && R2 > 0) // ()
            layout->paintMode = CurvedElementLayout::ConvexLens;
        else if (R1 > 0 && R2 < 0) // )(
            layout->paintMode = CurvedElementLayout::ConcaveLens;
        else if (R1 < 0 && R2 < 0) // ((
            layout->paintMode = CurvedElementLayout::ConvexConcaveLens;
        else if (R1 > 0 && R2 > 0) // ))
            layout->paintMode = CurvedElementLayout::ConcaveConvexLens;
        layout->init();
    }

    ELEMENT_LAYOUT_PAINT {
        if (layout) layout->paint(painter, nullptr, nullptr);
    }
}

//------------------------------------------------------------------------------
namespace ElemGaussApertureLensLayout {
    DECLARE_ELEMENT_LAYOUT_BEGIN
        QSharedPointer<CurvedElementLayout::Layout> layout;
    DECLARE_ELEMENT_LAYOUT_END

    ELEMENT_LAYOUT_INIT {
        HW = 10; HH = 40;
        auto lens = dynamic_cast<ElemGaussApertureLens*>(_element);
        if (!lens) return;
        layout.reset(new CurvedElementLayout::Layout(nullptr));
        layout->setHalfSize(HW, HH);
        layout->brush = getGrinBrush(HH);
        layout->paintMode = lens->focusT() > 0
                       ? CurvedElementLayout::PlanoConvexLens // |)
                       : CurvedElementLayout::PlanoConcaveLens; // |(
        layout->init();
    }

    ELEMENT_LAYOUT_PAINT {
        if (layout) layout->paint(painter, nullptr, nullptr);
    }
}

#endif // SCHEMA_LAYOUT_LENS_H
