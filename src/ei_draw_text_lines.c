#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "ei_tools.h"
#include "ei_draw.h"
#include "ei_draw_text_lines.h"

void ei_draw_text_lines(ei_surface_t	surface,
					const ei_point_t* where,
					char* text,
					const ei_font_t font,
					const ei_color_t* color,
					const ei_rect_t* clipper) {
    ei_point_t pt_src = {0, 0};
    ei_point_t pt_dst = *where;
    ei_size_t box;
    if (clipper) box = clipper->size;
    else box = (hw_surface_get_rect(surface)).size;

	char *line = NULL;
    char **tokens;
    size_t numtokens;
    tokens = strsplit(text, "\t\n", &numtokens);
	int width = 0;
	int height = 0;
    for (size_t i = 0; i < numtokens; i++) {
		// hw_text_compute_size(tokens[i], font, &width, &height);
        // printf("  token: \"%s\" %d %d\n", tokens[i], width, height);
		char *word = NULL;
	    char **wtokens;
	    size_t nwtokens;
	    wtokens = strsplit(tokens[i], " ", &nwtokens);
	    for (size_t j = 0; j < nwtokens; j++) {
            char buff[512] = {0};
            char tmp[512] = {0};
	        // printf("\tnewline - word: \"%s\"\n", wtokens[j]);
			strcat(buff, wtokens[j]);
            strcpy(tmp, buff);
            hw_text_compute_size(buff, font, &width, &height);
            free(wtokens[j]);
			while (j < nwtokens - 1) {
                strcat(tmp, " ");
                j++;
                strcat(tmp, wtokens[j]);
                hw_text_compute_size(tmp, font, &width, &height);
                if (pt_dst.x + width < box.width) {
                    strcpy(buff, tmp);
                } else {
                    j--;
                    break;
                }
                free(wtokens[j]);
			}
            // printf("%s\n", buff);
            ei_surface_t text_surface = hw_text_create_surface(buff, font, color);
            ei_rect_t clipper_text = hw_surface_get_rect(text_surface);
            ei_size_t draw_box = clipper_text.size;
            if (clipper) {
                if(clipper->top_left.x > pt_dst.x) {
                    draw_box.width += clipper->top_left.x - pt_dst.x;
                    pt_src.x += clipper->top_left.x - pt_dst.x;
                    pt_dst.x = clipper->top_left.x;
                }
                if(clipper->top_left.y > pt_dst.y) {
                    draw_box.height += clipper->top_left.y - pt_dst.y;
                    pt_src.y += clipper->top_left.y - pt_dst.y;
                    pt_dst.y = clipper->top_left.y;
                }
                //Not a bug: ei_copy_surface will cut the box to stay into the drawing area
                draw_box.width = clipper->top_left.x - pt_dst.x + clipper->size.width;
                draw_box.height = clipper->top_left.y - pt_dst.y + clipper->size.height;
            }
            ei_rect_t dst_rect = {pt_dst, draw_box};
            ei_rect_t src_rect = {pt_src, draw_box};
            ei_copy_surface(surface, &dst_rect, text_surface, &src_rect, EI_TRUE);
            hw_surface_free(text_surface);
            pt_dst.y += height;
		}
	    if (wtokens != NULL)
	        free(wtokens);
	    if (word != NULL) free(word);
		free(tokens[i]);
    }

    if (tokens != NULL)
        free(tokens);
    if (line != NULL) free(line);
}
