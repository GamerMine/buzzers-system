#ifndef BUZZERS_PAGE_H
#define BUZZERS_PAGE_H

class Page {
public:
    virtual ~Page() = default;

    virtual void show() {}
    virtual void hide() {}
    virtual void update() {}

    [[nodiscard]] bool dirty() const { return _dirty; }
    void markDirty() { this->_dirty = true; }

protected:
    bool _dirty = true;
};

#endif //BUZZERS_PAGE_H
