#pragma once

#include <libwidget/Elements.h>
#include <libwidget/model/TextModel.h>

namespace Widget
{

struct TextEditor : public Element
{
private:
    RefPtr<TextModel> _model;
    TextCursor _cursor{};

    RefPtr<ScrollBarElement> _vscrollbar;
    RefPtr<ScrollBarElement> _hscrollbar;

    int _vscroll_offset = 0;
    int _hscroll_offset = 0;

    bool _readonly = false;
    bool _overscroll = false;

    void update_scrollbar();

    void scroll_to_cursor();

    Math::Recti minimum_view_bound()
    {
        return bound().cutoff_right(ScrollBarElement::SIZE).cutoff_botton(ScrollBarElement::SIZE);
    }

    Math::Recti view_bound()
    {
        auto bound = Element::bound();

        if (document_bound().height() > minimum_view_bound().height())
        {
            bound = bound.cutoff_right(ScrollBarElement::SIZE);
        }

        if (document_bound().width() > minimum_view_bound().width())
        {
            bound = bound.cutoff_botton(ScrollBarElement::SIZE);
        }

        return bound;
    }

    Math::Recti document_bound()
    {
        return _model->bound(*font())
            .offset({-_hscroll_offset, -_vscroll_offset})
            .offset(bound().position());
    }

    Math::Recti vscrollbar_bound()
    {
        auto bound = Element::bound().take_right(ScrollBarElement::SIZE);

        if (document_bound().width() > minimum_view_bound().width())
        {
            bound = bound.cutoff_botton(ScrollBarElement::SIZE);
        }

        return bound;
    }

    Math::Recti hscrollbar_bound()
    {
        auto bound = Element::bound().take_bottom(ScrollBarElement::SIZE);

        if (document_bound().height() > minimum_view_bound().height())
        {
            bound = bound.cutoff_right(ScrollBarElement::SIZE);
        }

        return bound;
    }

public:
    void readonly(bool value)
    {
        _readonly = value;
        should_relayout();
        should_repaint();
    }

    void overscroll(bool value)
    {
        _overscroll = value;
        should_relayout();
        should_repaint();
    }

    TextEditor(RefPtr<TextModel> model);

    ~TextEditor();

    void update_model(RefPtr<TextModel> model);

    void paint(Graphic::Painter &, const Math::Recti &) override;

    void event(Event *event) override;

    void layout() override;
};

static inline RefPtr<TextEditor> texteditor(RefPtr<TextModel> model) { return make<TextEditor>(model); }

} // namespace Widget
