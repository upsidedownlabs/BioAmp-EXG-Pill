# Configuration file for the BioAmp-EXG-Pill documentation build.
#
# References
# https://github.com/zephyrproject-rtos/zephyr/blob/main/doc/conf.py
# https://www.sphinx-doc.org/en/master/usage/configuration.html


import os
from platform import release
import sys
# sys.path.insert(0, os.path.abspath('_ext'))
from pathlib import Path
import re

# sys.path.insert(0, os.path.abspath('.'))
import sphinx_rtd_theme

DOCS_BASE= Path(__file__).resolve().parents[0]

# -- Project information -----------------------------------------------------

project = 'BioAmp-EXG-Pill Docs'
copyright = '2022, Upside Down Labs'
author = 'Upside Down Labs'



# -- General configuration ---------------------------------------------------

extensions = [ 
    "sphinx.ext.autodoc",
    "sphinx.ext.githubpages",
]

# user starts in dark mode
default_dark_mode = True

# user starts in light mode
default_dark_mode = False

templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'
html_show_sphinx = False
html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]
html_theme_options = {
    "logo_only": True,
    "prev_next_buttons_location": None,
}
html_title = "BioAmp-EXG-Pill Documentation"
html_logo = str(DOCS_BASE / "_static" / "images" / "logo.png")
html_static_path = [str(DOCS_BASE / "_static")]
html_last_updated_fmt = "%b %d, %Y"
html_domain_indices = False
html_split_index = True
html_show_sourcelink = True
html_last_updated_fmt = "%b %d, %Y"
html_domain_indices = False
html_split_index = True
html_show_sourcelink = True
html_baseurl = 'https://upsidedownlabs.github.io/BioAmp-EXG-Pill/#/'

# parse version from 'VERSION' file
with open(DOCS_BASE  / "VERSION" ) as f:
    m = re.match(
        (
            r"^VERSION_MAJOR\s*=\s*(\d+)$\n"
            + r"^VERSION_MINOR\s*=\s*(\d+)$\n"
            + r"^PATCHLEVEL\s*=\s*(\d+)$\n"
            + r"^VERSION_TWEAK\s*=\s*\d+$\n"
            + r"^EXTRAVERSION\s*=\s*(.*)$"
        ),
        f.read(),
        re.MULTILINE,
    )

    if not m:
        sys.stderr.write("Warning: Could not extract kernel version\n")
        version = "Unknown"
    else:
        major, minor, patch, extra = m.groups(1)
        version = ".".join((major, minor, patch))
        if extra:
            version += "-" + extra

release = version

html_context = {
    "display_github": True,
    "github_user": "upsidedownlabs",
    "github_repo": "BioAmp-EXG-Pill",
    "github_version": "main",
    "conf_py_path": "/",
    "versions": (
        ("latest", "/"),
        ("v1.0", "/v1.0/"),
        ("v0.9", "/v0.9/"),
        ("v0.7", "/v0.7/"),
        ("v0.6", "/v0.6/"),
        ("v0.1", "/v0.1/"),
    )
}

# is_release = tags.has("release") 
# reference_prefix = ""

# if tags.has("publish"): 
#   reference_prefix = f"/{version}" if is_release else "/latest"
# docs_title = "docs / {}".format(version if is_release else "Latest")



# -- Options for LaTeX output ---------------------------------------------

latex_elements = {
    'extraclassoptions': 'oneside',
    "papersize": "a4paper",
    "maketitle": open(DOCS_BASE / "_static" / "latex" / "title.tex").read(),
    "preamble": open(DOCS_BASE  / "_static" / "latex" / "preamble.tex").read(),
    "fontpkg": r"\usepackage{charter}",
    "sphinxsetup": ",".join(
        (
            "verbatimwithframe=false",
            "VerbatimColor={HTML}{f0f2f4}",
            "InnerLinkColor={HTML}{2980b9}",
            "warningBgColor={HTML}{e9a499}",
            "warningborder=0pt",
            r"HeaderFamily=\rmfamily\bfseries",
        )
    ),
}


latex_logo = str(DOCS_BASE / "_static" / "images" / "logo-latex.pdf")
latex_documents = [
   ("index-tex", "bioamp-exg-pill.tex", "BioAmp-EXG-Pill Documentation", author, "manual"),
]


def setup(app):
    # theme customizations
    app.add_css_file("css/custom.css")


