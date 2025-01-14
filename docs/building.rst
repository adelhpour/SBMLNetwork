Building from source
====================

To build the SBMLNetwork library from source, you can follow the instructions provided in this section.

Prerequisites
-------------

Before building the SBMLNetwork library from source, you need to install the following dependencies:

- `CMake <https://cmake.org/download/>`_
- `libSBML static library <http://sbml.org/Software/libSBML>`_

It is highly recommended that you install the libSBML static library using its precompiled binary files. You can download the precompiled binary files from the `libroadrunner-deps release page <https://github.com/sys-bio/libroadrunner-deps/releases>`_.

Building the library
---------------------

To build the SBMLNetwork library from source, follow these steps:

1. Download or build from source libSBML static library binary files.
   
2. Clone the SBMLNetwork repository from GitHub:

.. code-block:: bash

   git clone https://github.com/adelhpour/SBMLNetwork.git

3. Change the directory to the SBMLNetwork repository:

.. code-block:: bash

   cd SBMLNetwork

4. Create a build directory:

.. code-block:: bash

   mkdir build
   cd build

5. Run the CMake command to configure the build:

.. code-block:: bash

   cmake -DCMAKE_INSTALL_PREFIX=/path/to/install -DCMAKE_BUILD_TYPE=Release -DDEPENDENCIES_INSTALL_PREFIX=/path/to/libsbml-static -DWITH_PYTHON=ON -DPYTHON_INSTALL_WITH_SETUP=ON ..

6. Run the make command to build the library:

.. code-block:: bash

   make

7. Run the make install command to install the library:

.. code-block:: bash

   make install

After following these steps, you will have successfully built the SBMLNetwork library from source.