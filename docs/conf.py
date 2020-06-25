# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import subprocess, os
# import sys
# sys.path.insert(0, os.path.abspath('.'))

# This code was adapted from https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/
def config_doxyfile(input_dir, output_dir):
    with open('Doxyfile.in', 'r') as f:
        data = f.read()

    data = data.replace('@DOXYGEN_INPUT_DIR@', input_dir)
    data = data.replace('@DOXYGEN_OUTPUT_DIR@', output_dir)

    with open('Doxyfile', 'w') as f:
        f.write(data)

breathe_projects = {}
if os.environ.get('READTHEDOCS', None):
    print('Hello, rtd!')
    config_doxyfile('..', 'build')
    subprocess.call('doxygen', None)
    path_exists = os.path.isdir('build/xml')
    print(f'does build/xml exist? {path_exists}')
    print('contents of build/xml')
    print('\n'.join(os.listdir('build/xml')))
    breathe_projects['hax'] = 'build/xml'
    breathe_default_project = 'hax'

# -- Project information -----------------------------------------------------

project = 'hax'
copyright = '2020, Josie Thompson'
author = 'Josie Thompson'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = ['breathe']
breathe_default_project = 'hax'

# Add any paths that contain templates here, relative to this directory.
templates_path = []

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
#html_theme = 'sphinx_rtd_theme'
html_theme = 'alabaster'
html_theme_options = {'fixed_sidebar': True}
html_sidebars = {'**': ['localtoc.html',
                        'searchbox.html']}
html_favicon = ''

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = []

