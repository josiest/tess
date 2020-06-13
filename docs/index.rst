.. hax documentation master file, created by
   sphinx-quickstart on Wed Jun 10 16:20:50 2020.

hax
===

hax is a library for working with hexagonally tiled maps. The library is largely
inspired by Amit Patel's `blog post <https://www.redblobgames.com/grids/hexagons/>`_
on the matter.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

math functions
==============
.. doxygenfunction:: hax::norm(const Point<T> &p) noexcept
.. doxygenfunction:: hax::norm(const Hex<T> &h) noexcept
.. doxygenfunction:: hax::normf(const Point<T> &p) noexcept
.. doxygenfunction:: hax::normf(const Hex<T> &h) noexcept


point
=====
.. doxygenclass:: hax::Point
   :members:
