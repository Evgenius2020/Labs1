#include <stdio.h>
#include <stdlib.h>
#include "Initialize.h"
#include "DataTypes.h"

#define isBetween(min, n, max) ((n >= min) && (n <= max))
#define MAX_VERTICES 5000
#define MAX_EDGES(n) (n*(n-1)/2) 

InitializationResult initialize(FILE* in) {
	InitializationResult result;
	int verticiesN, edgesN;
	short start, end; /* Vertices are ends of path. */
	short from;
	int i;
	Edge *buf;
	Edge* bufReversed;
	Vertex* verticies = NULL;

	/* Reading number of vertices */
	fscanf(in, "%d ", &verticiesN);
	if (!isBetween(0, verticiesN, MAX_VERTICES)) {
		result.statusCode = INIT_BAD_VERTICES_NUMBER;
		return result;
	}
	if (verticiesN == 0) {
		result.statusCode = INIT_BAD_VERTEX; /* Test(#1) */
		return result;
	}

	verticies = (Vertex*)malloc(sizeof(Vertex) * verticiesN);
	for (i = 0; i < verticiesN; i++) {
		verticies[i].viewed = UNVIEWED;
		verticies[i].distance = INF;
		verticies[i].edges = NULL;
		verticies[i].parent = NULL;
		verticies[i].index = i + 1;
		verticies[i].number = i + 1;
	}

	fscanf(in, "%hi %hi ", &start, &end);
	if ((!isBetween(0, start, verticiesN)) ||
		(!isBetween(0, end, verticiesN))) {
		result.statusCode = INIT_BAD_VERTEX;
		return result;
	}

	fscanf(in, "%d ", &edgesN);
	if (!isBetween(0, edgesN, MAX_EDGES(verticiesN))) {
		result.statusCode = INIT_BAD_EDGES_NUMBER;
		return result;
	}

	/* Reading edges from file. */
	for (i = 0; i < edgesN; i++) {
		buf = (Edge*)malloc(sizeof(Edge));
		if (EOF == fscanf(in, "%hi %hi %d ", &from, &buf->to, &buf->length)) {
			result.statusCode = INIT_BAD_LINES_NUMBER;
			return result;
		}
		if (from == buf->to) {
			free(buf);
			continue;
		}
		if ((!isBetween(1, from, verticiesN)) ||
			(!isBetween(1, buf->to, verticiesN))) {
			result.statusCode = INIT_BAD_VERTEX;
			return result;
		}
		if (!isBetween(0, buf->length, INT_MAX)) {
			result.statusCode = INIT_BAD_LENGTH;
			return result;
		}
		buf->next = verticies[from - 1].edges;
		verticies[from - 1].edges = buf;

		bufReversed = (Edge*)malloc(sizeof(Edge));
		bufReversed->to = from;
		bufReversed->length = buf->length;
		bufReversed->next = verticies[buf->to - 1].edges;
		verticies[buf->to - 1].edges = bufReversed;
	}

	result.vertices = verticies;
	result.verticesN = verticiesN;
	result.statusCode = INIT_SUCCESS;
	result.start = start;
	result.end = end;
	return result;
}