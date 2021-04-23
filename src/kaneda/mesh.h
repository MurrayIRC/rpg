#ifndef MESH_H
#define MESH_H

#include "math.h"
#include "shader.h"
#include "stretchy_buffer.h"

typedef struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 tex_coords;
} Vertex;

typedef struct Texture {
    uint32 id;
    const char *type;
} Texture;

typedef struct Mesh {
    Vertex *vertices;
    uint32 *indices;
    Texture *textures;

    uint32 vao;
    uint32 vbo;
    uint32 ebo;
} Mesh;

Mesh *mesh_create(Vertex *vertices, uint32 *indices, Texture *textures) {
    Mesh *mesh;
    mesh = malloc(sizeof(*mesh));
    mesh->vertices = vertices;
    mesh->indices = indices;
    mesh->textures = textures;

    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);
    glBindVertexArray(mesh->vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, sb_count(mesh->vertices) * sizeof(*mesh->vertices),
                 &mesh->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sb_count(mesh->indices) * sizeof(*mesh->indices),
                 &mesh->indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*mesh->vertices), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(*mesh->vertices),
                          (void *)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(*mesh->vertices),
                          (void *)offsetof(Vertex, tex_coords));

    glBindVertexArray(0);
}

void mesh_draw(Mesh *mesh, Shader &shader) {
    uint32 diffuse_number, specular_number = 1;

    for (uint32 i = 0; i < sb_count(mesh->textures); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        const char *number;
        const char *name = mesh->textures[i].type;
        if (strcmp(name, "texture_diffuse") == 0) {
            sprintf(number, "%u", diffuse_number++);
        } else if (strcmp(name, "texture_specular") == 0) {
            sprintf(number, "%u", specular_number++);
        }

        const char *mat = "material.";
        strcat(mat, name);
        strcat(mat, number);
        shader_set_float(shader, mat, i);
        glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sb_count(mesh->indices), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

#endif