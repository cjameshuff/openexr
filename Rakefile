#*******************************************************************************
#    Copyright (c) 2011, Christopher James Huff
#    All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#*******************************************************************************

require 'rubygems'
require 'rake'
require 'rake/clean'
require 'rake/gempackagetask'
require 'rake/extensiontask'

CLEAN.include('ext/*.o')
CLOBBER.include('ext/clibs')

# Define the files that will go into the gem
gem_files = FileList[
    "lib/*",
    "ext/*",
    "ext/clibs/include/**/*",
    "ext/clibs/lib/*",
    'README']
gem_files = gem_files.exclude("**/*.so", "**/*.o")

spec = Gem::Specification.new do |s|
    s.name              = "openexr"
    s.version           = "0.0.1"
    s.author            = "Christopher James Huff"
    s.email             = "cjameshuff@gmail.com"
    s.homepage          = "http://github.com/cjameshuff/openexr"
    s.platform          = Gem::Platform::RUBY
    s.summary           = "Bindings for openexr and libilmbase libraries"
    s.description       = "Bindings for openexr and libilmbase libraries"
    #s.rubyforge_project = "openexr"
    s.files             = gem_files
    s.require_path      = "lib"
    s.has_rdoc          = false
    s.add_dependency("rake")
end
# p spec.files

# Create a task for creating a ruby gem
Rake::GemPackageTask.new(spec) do |pkg|
    pkg.gem_spec = spec
    pkg.need_tar = true
end

Rake::ExtensionTask.new do |ext|
    ext.name = 'openexr_native'
    ext.ext_dir = 'ext'
    ext.lib_dir = 'lib'
    ext.source_pattern = "*.{c,cpp}"
    ext.gem_spec = spec
end

ILMVERSION = 'ilmbase-1.0.2'
EXRVERSION = 'openexr-1.7.0'
ZLIBVERSION = 'zlib-1.2.5'
file 'ext/clibs' do
    pwd = Dir.pwd
    sh "mkdir -p #{pwd}/ext/clibs"
    
    # Some further options to investigate:
    # --enable-osx-arch, --enable-osx-sdk
    sh "tar xf #{ILMVERSION}.tar.gz"
    sh "cd #{ILMVERSION}; patch < ../ilmconfigpatch.patch"
    sh "cd #{ILMVERSION}; ./configure --prefix=#{pwd}/ext/clibs --enable-static=yes --enable-shared=no; make; make install"
    
    sh "tar xf #{EXRVERSION}.tar.gz"
    sh "cd #{EXRVERSION}; patch < ../exrconfigpatch.patch"
    sh "cd #{EXRVERSION}; ./configure --prefix=#{pwd}/ext/clibs --enable-static=yes --enable-shared=no; make; make install"
    
    sh "tar xf #{ZLIBVERSION}.tar.gz"
    sh "cd #{ZLIBVERSION}; ./configure --prefix=#{pwd}/ext/clibs --static;"
    sh "cd #{ZLIBVERSION}; patch < ../zlibmfpatch.patch"
    sh "cd #{ZLIBVERSION}; make; make install"
end

Rake::Task["pkg"].prerequisites.insert(0, 'ext/clibs')
