require "rubygems"
require "rubygems/package_task"
require "rdoc/task"

require "rake/testtask"
Rake::TestTask.new do |t|
  t.libs << "test"
  t.test_files = FileList["test/**/*_test.rb"]
  t.verbose = true
end


task :default => ["test"]

# This builds the actual gem. For details of what all these options
# mean, and other ones you can add, check the documentation here:
#
#   http://rubygems.org/read/chapter/20
#
# CLEAN.include('ext/*.o')
# CLOBBER.include('ext/clibs')

# gem_files = FileList[
#     "lib/*",
#     "ext/*",
#     "ext/clibs/include/**/*",
#     "ext/clibs/lib/*",
#     'README']
# gem_files = gem_files.exclude("**/*.so", "**/*.o")

spec = Gem::Specification.new do |s|
  s.name              = "openexr"
  s.version           = "0.1.0"
  s.summary           = "Bindings for openexr and libilmbase libraries"
  s.author            = "Christopher James Huff"
  s.email             = "cjameshuff@gmail.com"
  s.homepage          = "http://github.com/cjameshuff/openexr"

  s.has_rdoc          = true
  s.extra_rdoc_files  = %w(README.markdown)
  s.rdoc_options      = %w(--main README.markdown)

  # Add any extra files to include in the gem
  s.files             = %w(README.markdown) + Dir.glob("{bin,test,lib,ext}/**/*")
  s.require_paths     = ["lib", "ext"]
  
  s.extensions << 'ext/extconf.rb'

  # If your tests use any gems, include them here
  # s.add_development_dependency("mocha") # for example
end

# This task actually builds the gem. We also regenerate a static
# .gemspec file, which is useful if something (i.e. GitHub) will
# be automatically building a gem for this project. If you're not
# using GitHub, edit as appropriate.
#
# To publish your gem online, install the 'gemcutter' gem; Read more 
# about that here: http://gemcutter.org/pages/gem_docs
Gem::PackageTask.new(spec) do |pkg|
  pkg.gem_spec = spec
end

desc "Build the gemspec file #{spec.name}.gemspec"
task :gemspec do
  file = File.dirname(__FILE__) + "/#{spec.name}.gemspec"
  File.open(file, "w") {|f| f << spec.to_ruby }
end

desc "Build gem locally"
task :build => :gemspec do
  system "gem build #{spec.name}.gemspec"
  FileUtils.mkdir_p "pkg"
  FileUtils.mv "#{spec.name}-#{spec.version}.gem", "pkg"
end
 
desc "Install gem locally"
task :install => :build do
  system "gem install pkg/#{spec.name}-#{spec.version}"
end

# If you don't want to generate the .gemspec file, just remove this line. Reasons
# why you might want to generate a gemspec:
#  - using bundler with a git source
#  - building the gem without rake (i.e. gem build blah.gemspec)
#  - maybe others?
task :package => :gemspec

# Generate documentation
RDoc::Task.new do |rd|
  rd.main = "README.markdown"
  rd.rdoc_files.include("README.markdown", "lib/**/*.rb", "ext/**/*.c", "ext/**/*.h")
  rd.rdoc_dir = "rdoc"
end

desc 'Clear out RDoc and generated packages'
task :clean => [:clobber_rdoc, :clobber_package] do
  rm "#{spec.name}.gemspec"
end

desc 'Tag the repository in git with gem version number'
task :tag => [:gemspec, :package] do
  if `git diff --cached`.empty?
    if `git tag`.split("\n").include?("v#{spec.version}")
      raise "Version #{spec.version} has already been released"
    end
    `git add #{File.expand_path("../#{spec.name}.gemspec", __FILE__)}`
    `git commit -m "Released version #{spec.version}"`
    `git tag v#{spec.version}`
    `git push --tags`
    `git push`
  else
    raise "Unstaged changes still waiting to be committed"
  end
end

desc "Tag and publish the gem to rubygems.org"
task :publish => :tag do
  `gem push pkg/#{spec.name}-#{spec.version}.gem`
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

Rake::Task["build"].prerequisites.insert(0, 'ext/clibs')

