#!/usr/bin/env python

import os
import shutil
import uuid

"https://pt.stackoverflow.com/questions/421175/como-deletar-arquivos-e-pastas-recursivamente-de-modo-seguro-com-python?rq=1"

def recursive_listing(path):
    files = []

    # r = root, d = directories, f = files
    for r, d, f in os.walk(path):
        for file in f:
            files.append(os.path.join(r, file))
        for dirs in d:
            files.append(os.path.join(r, dirs))

    list_ = [file for file in files]
    return list_


def secure_delete_recursive(path, steps=5):

    objects = recursive_listing(path)

    for obj in objects:
        # Para arquivos (gravando, renomeando e deletando)
        if os.path.isfile(obj):
            try:

                with open(obj, "ba+", buffering=0) as f:
                    data = f.tell()
                f.close()

                with open(obj, "br+", buffering=0) as f:
                    for i in range(steps):
                        f.seek(0, 0)
                        f.write(os.urandom(data))
                    f.seek(0)
                    for _ in range(data):
                        f.write(b'\x00' * len(data))

                name = str(uuid.uuid4())
                new_file_rename = os.path.join(os.path.split(obj)[0], name)
                os.rename(obj, new_file_rename)
                # Descomente a linha abaixo para deletar os arquivos recursivamente.
                os.remove(new_file_rename)

            except PermissionError as p:
                print(p)

    for obj in objects:
        # Para diretórios (renomeando e deletando)
        if os.path.isdir(obj):
            try:

                name = str(uuid.uuid4())
                new_file_rename = os.path.join(os.path.split(obj)[0], name)
                os.rename(obj, new_file_rename)
                # Descomente a linha abaixo para deletar as pastas recursivamente.
                # shutil.rmtree(new_file_rename, ignore_errors=False, onerror=None)

            except PermissionError as p:
                print(p)


if __name__ == '__main__':
    secure_delete_recursive('/tmp')
