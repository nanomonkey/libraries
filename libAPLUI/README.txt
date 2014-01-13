Copyright 2014 All Power Labs, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright 
notice, this list of conditions and the following disclaimer in the 
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=== All Power Labs User Interface Library v0.1 ===

This library is designed to bring a consistant user interface system to the
Power Pallet PCU.  It provides functions and structures for basic views such
as menus and dialogs.  There is also a multiplexer system for switching views.
The whole thing is written in C, but probly be re-written in C++ we can update
the code to support later versions of AVR libc that don't leak memory.

view.h - Basic view structure.  Views have callback functions for key handling
and updating the display.  There are also entrance and exit hooks.  The view
multiplexer handles loading, unloading, passing key events, and calling the
update function of the current view.

tree.h - a linked list tree structure and associated functions

menu.h - Menu is a sub-class of view that navigates a tree structure and
allows selection of an item from the tree.

dialog.h - Dialog is a sub-class of view that asks a question, usualy accepting
a yes or no response.

More documentation to follow... later.