#!/usr/bin/env python3
#

import argparse
from pathlib import Path

template = '''

.. _{}:

{}
{}

.. code-block:: cpp

'''

dir_template = '''

.. _{link_name}:

{caption_name}
{title}

.. toctree::
   :maxdepth: 1

'''	      


def main(root_source_directory, source_subdirectories, target_directory):
    paths = []
    for d in source_subdirectories:
        path = Path(root_source_directory, d)
        paths += list(Path(path).rglob("*.h"))
        paths += list(Path(path).rglob("*.cpp"))

    for path in paths:
        rel_path = path.relative_to(root_source_directory)
        if rel_path.parent == '..':
            raise ValueError('root_source_directory must contain all files')
        
        rst_parts = [target_directory]
        for part in rel_path.parts:
            rst_parts.append(part)
        rst_parts[-1] += '.rst'
        rst_path = Path(*rst_parts)
        
        rst_parent = rst_path.parent
        if not rst_parent.exists():
            rst_parent.mkdir(parents=True)
            
        src_name = rel_path
        name = src_name.name
            
        with open(rst_path, 'w') as wfp:
            header = '=' * (4 + len(src_name.as_posix()))
            wfp.write(template.format(src_name, name, header))
            with open(path, 'r') as rfp:
                for line in rfp:
                    line = line.replace('\t', (' ' * 8))
                    wfp.write('    ' + line)

    for path in (p for p in Path(target_directory).rglob('*') if p.is_dir()):
        link_name = path.relative_to(target_directory).as_posix()
        caption_name = path.stem
        rst_file = path / 'dir.rst'
        dirs = list(Path(p) for p in path.glob('*') if p.is_dir())
        
        files = list(Path(p) for p in path.glob('*.h.rst'))
        files += list(Path(p) for p in path.glob('*.cpp.rst'))

        with open(rst_file, 'w') as wfp:
            wfp.write(dir_template.format(caption_name=caption_name,
                                          link_name=link_name,
                                          title=('=' * (len(caption_name) + 4))))

            for dir in sorted(dirs):
                dirname = dir.relative_to(path).as_posix()
                wfp.write('   {}/ <{}>\n'.format(dirname, dirname + '/dir'))
                
            for file in sorted(files):
                wfp.write('   ' + file.stem + '\n')
                
            
    

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-r', '--root-source-directory', type=str, default='.',
                        help='Root source subdirectory')
    parser.add_argument('-s', '--source-subdirectories', type=str, nargs='+', default='.',
                        help='Source subdirectories')
    parser.add_argument('-t', '--target-directory', type=str, default='.',
                        help='Target directory')
    main(**vars(parser.parse_args()))
