#ifndef TEMPLATE_H
#define TEMPLATE_H

/* Note: When writing templates, do not separate
 *       the prototype and implementations into
 *       separate files. Keep it all in the header
 *       file. :)
 */

template <typename _Tp>
class Clazz {
    public:

    static _Tp echo(_Tp __a) {
        return __a;
    }
};

#endif
