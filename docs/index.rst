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
--------------
.. doxygenfunction:: hax::norm(const Point<T> &p) noexcept

|

.. doxygenfunction:: hax::normf(const Point<T> &p) noexcept

|

.. doxygenfunction:: hax::sqnorm(const Point<T> &p) noexcept

|

.. doxygenfunction:: hax::hex_norm(const Hex<T> &h) noexcept

|

.. doxygenfunction:: hax::hex_round(const Hex<R> &h) noexcept

|

.. doxygenfunction:: hax::line

|

.. doxygenfunction:: hax::hex_range

point
-----
.. doxygenclass:: hax::Point
   :members:

hex
---
.. doxygenclass:: hax::Hex
   :members:

basis
-----
.. doxygenclass:: hax::Basis
   :members:
