/* Configure your desired default key bindings. */

#define ALIAS(name) .alias = name,

static const char *keymaps[] = {
	NULL
};

static const KeyBinding bindings_basic[] = {
	{ "<C-z>",              ALIAS("<vis-suspend>")                                         },
	{ "<Down>",             ALIAS("<vis-motion-line-down>")                                },
	{ "<End>",              ALIAS("<vis-motion-line-end>")                                 },
	{ "<Home>",             ALIAS("<vis-motion-line-begin>")                               },
	{ "<Left>",             ALIAS("<vis-motion-char-prev>")                                },
	{ "<PageDown>",         ALIAS("<vis-window-page-down>")                                },
	{ "<PageUp>",           ALIAS("<vis-window-page-up>")                                  },
	{ "<Right>",            ALIAS("<vis-motion-char-next>")                                },
	{ "<S-Left>",           ALIAS("<vis-motion-bigword-start-prev>")                       },
	{ "<S-PageDown>",       ALIAS("<vis-window-halfpage-down>")                            },
	{ "<S-PageUp>",         ALIAS("<vis-window-halfpage-up>")                              },
	{ "<S-Right>",          ALIAS("<vis-motion-bigword-start-next>")                       },
	{ "<Up>",               ALIAS("<vis-motion-line-up>")                                  },
	{ 0                     /* empty last element, array terminator */                     },
};

static const KeyBinding bindings_motions[] = {
	{ "g|",                 ALIAS("<vis-motion-column>")                                   },
	{ "[{",                 ALIAS("<vis-motion-block-start>")                              },
	{ "]}",                 ALIAS("<vis-motion-block-end>")                                },
	{ "[(",                 ALIAS("<vis-motion-parenthesis-start>")                        },
	{ "])",                 ALIAS("<vis-motion-parenthesis-end>")                          },
	{ "$",                  ALIAS("<vis-motion-line-end>")                                 },
	{ "^",                  ALIAS("<vis-motion-line-start>")                               },
	{ "}",                  ALIAS("<vis-motion-paragraph-next>")                           },
	{ "{",                  ALIAS("<vis-motion-paragraph-prev>")                           },
	{ "%",                  ALIAS("<vis-motion-percent>")                                  },
	{ "#",                  ALIAS("<vis-motion-search-word-backward>")                     },
	{ "*",                  ALIAS("<vis-motion-search-word-forward>")                      },
	{ ")",                  ALIAS("<vis-motion-sentence-next>")                            },
	{ "(",                  ALIAS("<vis-motion-sentence-prev>")                            },
	{ "?",                  ALIAS("<vis-search-backward>")                                 },
	{ "/",                  ALIAS("<vis-search-forward>")                                  },
	{ ";",                  ALIAS("<vis-motion-totill-repeat>")                            },
	{ ",",                  ALIAS("<vis-motion-totill-reverse>")                           },
	{ "+",                  ALIAS("j^")                                                    },
	{ "-",                  ALIAS("k^")                                                    },
	{ "B",                  ALIAS("<vis-motion-bigword-start-prev>")                       },
	{ "b",                  ALIAS("<vis-motion-word-start-prev>")                          },
	{ "E",                  ALIAS("<vis-motion-bigword-end-next>")                         },
	{ "e",                  ALIAS("<vis-motion-word-end-next>")                            },
	{ "F",                  ALIAS("<vis-motion-to-line-left>")                             },
	{ "f",                  ALIAS("<vis-motion-to-line-right>")                            },
	{ "go",                 ALIAS("<vis-motion-byte>")                                     },
	{ "gH",                 ALIAS("<vis-motion-byte-left>")                                },
	{ "gL",                 ALIAS("<vis-motion-byte-right>")                               },
	{ "gh",                 ALIAS("<vis-motion-codepoint-prev>")                           },
	{ "gl",                 ALIAS("<vis-motion-codepoint-next>")                           },
	{ "g0",                 ALIAS("<vis-motion-screenline-begin>")                         },
	{ "g_",                 ALIAS("<vis-motion-line-finish>")                              },
	{ "G",                  ALIAS("<vis-motion-line-last>")                                },
	{ "g$",                 ALIAS("<vis-motion-screenline-end>")                           },
	{ "gE",                 ALIAS("<vis-motion-bigword-end-prev>")                         },
	{ "ge",                 ALIAS("<vis-motion-word-end-prev>")                            },
	{ "gg",                 ALIAS("<vis-motion-line-first>")                               },
	{ "gj",                 ALIAS("<vis-motion-screenline-down>")                          },
	{ "gk",                 ALIAS("<vis-motion-screenline-up>")                            },
	{ "gm",                 ALIAS("<vis-motion-screenline-middle>")                        },
	{ "h",                  ALIAS("<vis-motion-char-prev>")                                },
	{ "H",                  ALIAS("<vis-motion-window-line-top>")                          },
	{ "j",                  ALIAS("<vis-motion-line-down>")                                },
	{ "k",                  ALIAS("<vis-motion-line-up>")                                  },
	{ "l",                  ALIAS("<vis-motion-char-next>")                                },
	{ "L",                  ALIAS("<vis-motion-window-line-bottom>")                       },
	{ "M",                  ALIAS("<vis-motion-window-line-middle>")                       },
	{ "n",                  ALIAS("<vis-motion-search-repeat-forward>")                    },
	{ "N",                  ALIAS("<vis-motion-search-repeat-backward>")                   },
	{ "T",                  ALIAS("<vis-motion-till-line-left>")                           },
	{ "t",                  ALIAS("<vis-motion-till-line-right>")                          },
	{ "W",                  ALIAS("<vis-motion-bigword-start-next>")                       },
	{ "w",                  ALIAS("<vis-motion-word-start-next>")                          },
	{ 0                     /* empty last element, array terminator */                     },
};

static const KeyBinding bindings_textobjects[] = {
	{ "a<",                 ALIAS("<vis-textobject-angle-bracket-outer>")                  },
	{ "a`",                 ALIAS("<vis-textobject-backtick-outer>")                       },
	{ "a{",                 ALIAS("<vis-textobject-curly-bracket-outer>")                  },
	{ "a(",                 ALIAS("<vis-textobject-parenthesis-outer>")                    },
	{ "a\"",                ALIAS("<vis-textobject-quote-outer>")                          },
	{ "a\'",                ALIAS("<vis-textobject-single-quote-outer>")                   },
	{ "a[",                 ALIAS("<vis-textobject-square-bracket-outer>")                 },
	{ "a>",                 ALIAS("a<")                                                    },
	{ "a)",                 ALIAS("a(")                                                    },
	{ "a]",                 ALIAS("a[")                                                    },
	{ "a}",                 ALIAS("a{")                                                    },
	{ "ab",                 ALIAS("a(")                                                    },
	{ "aB",                 ALIAS("a{")                                                    },
	{ "al",                 ALIAS("<vis-textobject-line-outer>")                           },
	{ "ap",                 ALIAS("<vis-textobject-paragraph-outer>")                      },
	{ "as",                 ALIAS("<vis-textobject-sentence>")                             },
	{ "a<Tab>",             ALIAS("<vis-textobject-indentation>")                          },
	{ "aW",                 ALIAS("<vis-textobject-bigword-outer>")                        },
	{ "aw",                 ALIAS("<vis-textobject-word-outer>")                           },
	{ "gN",                 ALIAS("<vis-textobject-search-backward>")                      },
	{ "gn",                 ALIAS("<vis-textobject-search-forward>")                       },
	{ "i<",                 ALIAS("<vis-textobject-angle-bracket-inner>")                  },
	{ "i`",                 ALIAS("<vis-textobject-backtick-inner>")                       },
	{ "i{",                 ALIAS("<vis-textobject-curly-bracket-inner>")                  },
	{ "i(",                 ALIAS("<vis-textobject-parenthesis-inner>")                    },
	{ "i\"",                ALIAS("<vis-textobject-quote-inner>")                          },
	{ "i\'",                ALIAS("<vis-textobject-single-quote-inner>")                   },
	{ "i[",                 ALIAS("<vis-textobject-square-bracket-inner>")                 },
	{ "i>",                 ALIAS("i<")                                                    },
	{ "i)",                 ALIAS("i(")                                                    },
	{ "i]",                 ALIAS("i[")                                                    },
	{ "i}",                 ALIAS("i{")                                                    },
	{ "ib",                 ALIAS("i(")                                                    },
	{ "iB",                 ALIAS("i{")                                                    },
	{ "il",                 ALIAS("<vis-textobject-line-inner>")                           },
	{ "ip",                 ALIAS("<vis-textobject-paragraph>")                            },
	{ "is",                 ALIAS("<vis-textobject-sentence>")                             },
	{ "i<Tab>",             ALIAS("<vis-textobject-indentation>")                          },
	{ "iW",                 ALIAS("<vis-textobject-bigword-inner>")                        },
	{ "iw",                 ALIAS("<vis-textobject-word-inner>")                           },
	{ 0                     /* empty last element, array terminator */                     },
};

static const KeyBinding bindings_selections[] = {
	{ "m",                  ALIAS("<vis-selections-save>")                                 },
	{ "M",                  ALIAS("<vis-selections-restore>")                              },
	{ "|",                  ALIAS("<vis-selections-union>")                                },
	{ "&",                  ALIAS("<vis-selections-intersect>")                            },
	{ "~",                  ALIAS("<vis-selections-complement>")                           },
	{ "\\",                 ALIAS("<vis-selections-minus>")                                },
	{ "_",                  ALIAS("<vis-selections-trim>")                                 },
	{ "<S-Tab>",            ALIAS("<vis-selections-align-indent-right>")                   },
	{ "<Tab>",              ALIAS("<vis-selections-align-indent-left>")                    },
	{ "g<",                 ALIAS("<vis-jumplist-prev>")                                   },
	{ "gs",                 ALIAS("<vis-jumplist-save>")                                   },
	{ "g>",                 ALIAS("<vis-jumplist-next>")                                   },
	{ 0                     /* empty last element, array terminator */                     },
};

static const KeyBinding bindings_operators[] = {
	{ "0",                  ALIAS("<vis-count-zero>")                                      },
	{ "1",                  ALIAS("<vis-count-one>")                                       },
	{ "2",                  ALIAS("<vis-count-two>")                                       },
	{ "3",                  ALIAS("<vis-count-three>")                                     },
	{ "4",                  ALIAS("<vis-count-four>")                                      },
	{ "5",                  ALIAS("<vis-count-five>")                                      },
	{ "6",                  ALIAS("<vis-count-six>")                                       },
	{ "7",                  ALIAS("<vis-count-seven>")                                     },
	{ "8",                  ALIAS("<vis-count-eight>")                                     },
	{ "9",                  ALIAS("<vis-count-nine>")                                      },
	{ "=",                  ALIAS(":|fmt<Enter>")                                          },
	{ "<",                  ALIAS("<vis-operator-shift-left>")                             },
	{ ">",                  ALIAS("<vis-operator-shift-right>")                            },
	{ "\"",                 ALIAS("<vis-register>")                                        },
	{ "'",                  ALIAS("<vis-mark>")                                            },
	{ "c",                  ALIAS("<vis-operator-change>")                                 },
	{ "d",                  ALIAS("<vis-operator-delete>")                                 },
	{ "g~",                 ALIAS(":|tr '[:lower:][:upper:]' '[:upper:][:lower:]'<Enter>") },
	{ "gu",                 ALIAS(":|awk '{printf \"%s\", tolower($0)}'<Enter>")           },
	{ "gU",                 ALIAS(":|awk '{printf \"%s\", toupper($0)}'<Enter>")           },
	{ "p",                  ALIAS("<vis-put-after>")                                       },
	{ "P",                  ALIAS("<vis-put-before>")                                      },
	{ "y",                  ALIAS("<vis-operator-yank>")                                   },
	{ 0                     /* empty last element, array terminator */                     },
};

static const KeyBinding bindings_normal[] = {
	{ "a",                  ALIAS("<vis-append-char-next>")                                },
	{ "A",                  ALIAS("<vis-append-line-end>")                                 },
	{ "@",                  ALIAS("<vis-macro-replay>")                                    },
	{ ":",                  ALIAS("<vis-prompt-show>")                                     },
	{ ".",                  ALIAS("<vis-repeat>")                                          },
	{ "C",                  ALIAS("c$")                                                    },
	{ "<C-b>",              ALIAS("<PageUp>")                                              },
	{ "<C-c>",              ALIAS("<vis-selections-remove-column>")                        },
	{ "<C-d>",              ALIAS("<vis-selection-next>")                                  },
	{ "<C-e>",              ALIAS("<vis-window-slide-up>")                                 },
	{ "<C-f>",              ALIAS("<PageDown>")                                            },
	{ "<C-j>",              ALIAS("<vis-selection-new-lines-below>")                       },
	{ "<C-k>",              ALIAS("<vis-selection-new-lines-above>")                       },
	{ "<C-l>",              ALIAS("<vis-selections-remove-column-except>")                 },
	{ "<C-n>",              ALIAS("viw")                                                   },
	{ "<C-p>",              ALIAS("<vis-selections-remove-last>")                          },
	{ "<C-r>",              ALIAS("<vis-redo>")                                            },
	{ "<C-u>",              ALIAS("<vis-selection-prev>")                                  },
	{ "<C-w>c",             ALIAS(":q<Enter>")                                             },
	{ "<C-w>h",             ALIAS("<C-w>k")                                                },
	{ "<C-w>j",             ALIAS("<vis-window-next>")                                     },
	{ "<C-w>k",             ALIAS("<vis-window-prev>")                                     },
	{ "<C-w>l",             ALIAS("<C-w>j")                                                },
	{ "<C-w>n",             ALIAS(":open<Enter>")                                          },
	{ "<C-w>s",             ALIAS(":split<Enter>")                                         },
	{ "<C-w>v",             ALIAS(":vsplit<Enter>")                                        },
	{ "<C-y>",              ALIAS("<vis-window-slide-down>")                               },
	{ "D",                  ALIAS("d$")                                                    },
	{ "<Escape>",           ALIAS("<vis-mode-normal-escape>")                              },
	{ "<F1>",               ALIAS(":help<Enter>")                                          },
	{ "ga",                 ALIAS("<vis-unicode-info>")                                    },
	{ "g8",                 ALIAS("<vis-utf8-info>")                                       },
	{ "g-",                 ALIAS("<vis-earlier>")                                         },
	{ "g+",                 ALIAS("<vis-later>")                                           },
	{ "gn",                 ALIAS("vgn")                                                   },
	{ "gN",                 ALIAS("vgN")                                                   },
	{ "gv",                 ALIAS("v'^M")                                                  },
	{ "I",                  ALIAS("<vis-insert-line-start>")                               },
	{ "i",                  ALIAS("<vis-mode-insert>")                                     },
	{ "J",                  ALIAS("<vis-join-lines>")                                      },
	{ "gJ",                 ALIAS("<vis-join-lines-trim>")                                 },
	{ "<M-C-j>",            ALIAS("<vis-selection-new-lines-below-last>")                  },
	{ "<M-C-k>",            ALIAS("<vis-selection-new-lines-above-first>")                 },
	{ "O",                  ALIAS("<vis-open-line-above>")                                 },
	{ "o",                  ALIAS("<vis-open-line-below>")                                 },
	{ "q",                  ALIAS("<vis-macro-record>")                                    },
	{ "R",                  ALIAS("<vis-mode-replace>")                                    },
	{ "r",                  ALIAS("<vis-replace-char>")                                    },
	{ "S",                  ALIAS("^c$")                                                   },
	{ "s",                  ALIAS("cl")                                                    },
	{ "<Tab>",              ALIAS("<vis-selections-align>")                                },
	{ "u",                  ALIAS("<vis-undo>")                                            },
	{ "v",                  ALIAS("<vis-mode-visual-charwise>")                            },
	{ "V",                  ALIAS("<vis-mode-visual-linewise>")                            },
	{ "x",                  ALIAS("<vis-delete-char-next>")                                },
	{ "X",                  ALIAS("dh")                                                    },
	{ "Y",                  ALIAS("y$")                                                    },
	{ "zb",                 ALIAS("<vis-window-redraw-bottom>")                            },
	{ "ZQ",                 ALIAS(":q!<Enter>")                                            },
	{ "zt",                 ALIAS("<vis-window-redraw-top>")                               },
	{ "zz",                 ALIAS("<vis-window-redraw-center>")                            },
	{ "ZZ",                 ALIAS(":wq<Enter>")                                            },
	{ 0                     /* empty last element, array terminator */                     },
};

static const KeyBinding bindings_visual[] = {
	{ "A",                  ALIAS("<vis-selection-new-lines-end>")                         },
	{ "@",                  ALIAS("<vis-macro-replay>")                                    },
	{ ":",                  ALIAS("<vis-prompt-show>")                                     },
	{ "-",                  ALIAS("<vis-selections-rotate-left>")                          },
	{ "+",                  ALIAS("<vis-selections-rotate-right>")                         },
	{ "<",                  ALIAS("<vis-operator-shift-left>gv")                           },
	{ ">",                  ALIAS("<vis-operator-shift-right>gv")                          },
	{ "<C-a>",              ALIAS("<vis-selection-new-match-all>")                         },
	{ "<C-b>",              ALIAS("<PageUp>")                                              },
	{ "<C-c>",              ALIAS("<vis-selections-remove-column>")                        },
	{ "<C-d>",              ALIAS("<vis-selection-next>")                                  },
	{ "<C-f>",              ALIAS("<PageDown>")                                            },
	{ "<C-j>",              ALIAS("<C-d>")                                                 },
	{ "<C-k>",              ALIAS("<C-u>")                                                 },
	{ "<C-l>",              ALIAS("<vis-selections-remove-column-except>")                 },
	{ "<C-n>",              ALIAS("<vis-selection-new-match-next>")                        },
	{ "<C-p>",              ALIAS("<vis-selections-remove-last>")                          },
	{ "<C-u>",              ALIAS("<vis-selection-prev>")                                  },
	{ "<C-x>",              ALIAS("<vis-selection-new-match-skip>")                        },
	{ "<Escape>",           ALIAS("<vis-mode-visual-escape>")                              },
	{ "I",                  ALIAS("<vis-selection-new-lines-begin>")                       },
	{ "J",                  ALIAS("<vis-join-lines>")                                      },
	{ "gJ",                 ALIAS("<vis-join-lines-trim>")                                 },
	{ "o",                  ALIAS("<vis-selection-flip>")                                  },
	{ "q",                  ALIAS("<vis-macro-record>")                                    },
	{ "r",                  ALIAS("<vis-replace-char>")                                    },
	{ "s",                  ALIAS("c")                                                     },
	{ "u",                  ALIAS("gu<Escape>")                                            },
	{ "U",                  ALIAS("gU<Escape>")                                            },
	{ "V",                  ALIAS("<vis-mode-visual-linewise>")                            },
	{ "v",                  ALIAS("<Escape>")                                              },
	{ "x",                  ALIAS("d")                                                     },
	{ 0                     /* empty last element, array terminator */                     },
};

static const KeyBinding bindings_visual_line[] = {
	{ "v",                  ALIAS("<vis-mode-visual-charwise>")                            },
	{ "V",                  ALIAS("<vis-mode-normal>")                                     },
	{ 0                     /* empty last element, array terminator */                     },
};

static const KeyBinding bindings_readline[] = {
	{ "<Backspace>",        ALIAS("<vis-delete-char-prev>")                                },
	{ "<C-c>",              ALIAS("<Escape>")                                              },
	{ "<C-d>",              ALIAS("<vis-delete-char-next>")                                },
	{ "<C-h>",              ALIAS("<Backspace>")                                           },
	{ "<C-u>",              ALIAS("<vis-delete-line-begin>")                               },
	{ "<C-v>",              ALIAS("<vis-insert-verbatim>")                                 },
	{ "<C-w>",              ALIAS("<vis-delete-word-prev>")                                },
	{ "<C-e>",              ALIAS("<vis-motion-line-end>")                                 },
	{ "<C-a>",              ALIAS("<vis-motion-line-start>")                               },
	{ "<Delete>",           ALIAS("<vis-delete-char-next>")                                },
	{ "<Escape>",           ALIAS("<vis-mode-normal>")                                     },
	{ 0                     /* empty last element, array terminator */                     },
};

static const KeyBinding bindings_insert[] = {
	{ "<C-d>",              ALIAS("<vis-operator-shift-left><vis-operator-shift-left>")    },
	{ "<C-i>",              ALIAS("<Tab>")                                                 },
	{ "<C-j>",              ALIAS("<vis-insert-verbatim>u000a")                            },
	{ "<C-m>",              ALIAS("<Enter>")                                               },
	{ "<C-r>",              ALIAS("<vis-insert-register>")                                 },
	{ "<C-t>",              ALIAS("<vis-operator-shift-right><vis-operator-shift-right>")  },
	{ "<C-x><C-e>",         ALIAS("<vis-window-slide-up>")                                 },
	{ "<C-x><C-y>",         ALIAS("<vis-window-slide-down>")                               },
	{ "<Enter>",            ALIAS("<vis-insert-newline>")                                  },
	{ "<Escape>",           ALIAS("<vis-mode-normal>")                                     },
	{ "<S-Tab>",            ALIAS("<vis-selections-align-indent-left>")                    },
	{ "<Tab>",              ALIAS("<vis-insert-tab>")                                      },
	{ 0                     /* empty last element, array terminator */                     },
};

static const KeyBinding bindings_replace[] = {
	{ 0                     /* empty last element, array terminator */                     },
};

/* For each mode we list a all key bindings, if a key is bound in more than
 * one array the first definition is used and further ones are ignored. */
static const KeyBinding **default_bindings[] = {
	[VIS_MODE_OPERATOR_PENDING] = (const KeyBinding*[]){
		bindings_operators,
		bindings_textobjects,
		bindings_motions,
		bindings_basic,
		NULL,
	},
	[VIS_MODE_NORMAL] = (const KeyBinding*[]){
		bindings_normal,
		bindings_selections,
		bindings_operators,
		bindings_motions,
		bindings_basic,
		NULL,
	},
	[VIS_MODE_VISUAL] = (const KeyBinding*[]){
		bindings_visual,
		bindings_selections,
		bindings_textobjects,
		bindings_operators,
		bindings_motions,
		bindings_basic,
		NULL,
	},
	[VIS_MODE_VISUAL_LINE] = (const KeyBinding*[]){
		bindings_visual_line,
		NULL,
	},
	[VIS_MODE_INSERT] = (const KeyBinding*[]){
		bindings_insert,
		bindings_readline,
		bindings_basic,
		NULL,
	},
	[VIS_MODE_REPLACE] = (const KeyBinding*[]){
		bindings_replace,
		NULL,
	},
};

static const KeyAction vis_action[] = {
	{
		"vis-suspend",
		VIS_HELP("Suspend the editor")
		suspend,
	},
	{
		"vis-motion-char-prev",
		VIS_HELP("Move cursor left, to the previous character")
		movement, { .i = VIS_MOVE_CHAR_PREV }
	},
	{
		"vis-motion-char-next",
		VIS_HELP("Move cursor right, to the next character")
		movement, { .i = VIS_MOVE_CHAR_NEXT }
	},
	{
		"vis-motion-line-char-prev",
		VIS_HELP("Move cursor left, to the previous character on the same line")
		movement, { .i = VIS_MOVE_LINE_CHAR_PREV }
	},
	{
		"vis-motion-line-char-next",
		VIS_HELP("Move cursor right, to the next character on the same line")
		movement, { .i = VIS_MOVE_LINE_CHAR_NEXT }
	},
	{
		"vis-motion-codepoint-prev",
		VIS_HELP("Move to the previous Unicode codepoint")
		movement, { .i = VIS_MOVE_CODEPOINT_PREV }
	},
	{
		"vis-motion-codepoint-next",
		VIS_HELP("Move to the next Unicode codepoint")
		movement, { .i = VIS_MOVE_CODEPOINT_NEXT }
	},
	{
		"vis-motion-word-start-prev",
		VIS_HELP("Move cursor words backwards")
		movement, { .i = VIS_MOVE_WORD_START_PREV }
	},
	{
		"vis-motion-word-start-next",
		VIS_HELP("Move cursor words forwards")
		movement, { .i = VIS_MOVE_WORD_START_NEXT }
	},
	{
		"vis-motion-word-end-prev",
		VIS_HELP("Move cursor backwards to the end of word")
		movement, { .i = VIS_MOVE_WORD_END_PREV }
	},
	{
		"vis-motion-word-end-next",
		VIS_HELP("Move cursor forward to the end of word")
		movement, { .i = VIS_MOVE_WORD_END_NEXT }
	},
	{
		"vis-motion-bigword-start-prev",
		VIS_HELP("Move cursor WORDS backwards")
		movement, { .i = VIS_MOVE_LONGWORD_START_PREV }
	},
	{
		"vis-motion-bigword-start-next",
		VIS_HELP("Move cursor WORDS forwards")
		movement, { .i = VIS_MOVE_LONGWORD_START_NEXT }
	},
	{
		"vis-motion-bigword-end-prev",
		VIS_HELP("Move cursor backwards to the end of WORD")
		movement, { .i = VIS_MOVE_LONGWORD_END_PREV }
	},
	{
		"vis-motion-bigword-end-next",
		VIS_HELP("Move cursor forward to the end of WORD")
		movement, { .i = VIS_MOVE_LONGWORD_END_NEXT }
	},
	{
		"vis-motion-line-up",
		VIS_HELP("Move cursor line upwards")
		movement, { .i = VIS_MOVE_LINE_UP }
	},
	{
		"vis-motion-line-down",
		VIS_HELP("Move cursor line downwards")
		movement, { .i = VIS_MOVE_LINE_DOWN }
	},
	{
		"vis-motion-line-start",
		VIS_HELP("Move cursor to first non-blank character of the line")
		movement, { .i = VIS_MOVE_LINE_START }
	},
	{
		"vis-motion-line-finish",
		VIS_HELP("Move cursor to last non-blank character of the line")
		movement, { .i = VIS_MOVE_LINE_FINISH }
	},
	{
		"vis-motion-line-begin",
		VIS_HELP("Move cursor to first character of the line")
		movement, { .i = VIS_MOVE_LINE_BEGIN }
	},
	{
		"vis-motion-line-end",
		VIS_HELP("Move cursor to end of the line")
		movement, { .i = VIS_MOVE_LINE_END }
	},
	{
		"vis-motion-screenline-up",
		VIS_HELP("Move cursor screen/display line upwards")
		movement, { .i = VIS_MOVE_SCREEN_LINE_UP }
	},
	{
		"vis-motion-screenline-down",
		VIS_HELP("Move cursor screen/display line downwards")
		movement, { .i = VIS_MOVE_SCREEN_LINE_DOWN }
	},
	{
		"vis-motion-screenline-begin",
		VIS_HELP("Move cursor to beginning of screen/display line")
		movement, { .i = VIS_MOVE_SCREEN_LINE_BEGIN }
	},
	{
		"vis-motion-screenline-middle",
		VIS_HELP("Move cursor to middle of screen/display line")
		movement, { .i = VIS_MOVE_SCREEN_LINE_MIDDLE }
	},
	{
		"vis-motion-screenline-end",
		VIS_HELP("Move cursor to end of screen/display line")
		movement, { .i = VIS_MOVE_SCREEN_LINE_END }
	},
	{
		"vis-motion-percent",
		VIS_HELP("Move to count % of file or matching item")
		percent
	},
	{
		"vis-motion-byte",
		VIS_HELP("Move to absolute byte position")
		movement, { .i = VIS_MOVE_BYTE }
	},
	{
		"vis-motion-byte-left",
		VIS_HELP("Move count bytes to the left")
		movement, { .i = VIS_MOVE_BYTE_LEFT }
	},
	{
		"vis-motion-byte-right",
		VIS_HELP("Move count bytes to the right")
		movement, { .i = VIS_MOVE_BYTE_RIGHT }
	},
	{
		"vis-motion-paragraph-prev",
		VIS_HELP("Move cursor paragraph backward")
		movement, { .i = VIS_MOVE_PARAGRAPH_PREV }
	},
	{
		"vis-motion-paragraph-next",
		VIS_HELP("Move cursor paragraph forward")
		movement, { .i = VIS_MOVE_PARAGRAPH_NEXT }
	},
	{
		"vis-motion-sentence-prev",
		VIS_HELP("Move cursor sentence backward")
		movement, { .i = VIS_MOVE_SENTENCE_PREV }
	},
	{
		"vis-motion-sentence-next",
		VIS_HELP("Move cursor sentence forward")
		movement, { .i = VIS_MOVE_SENTENCE_NEXT }
	},
	{
		"vis-motion-block-start",
		VIS_HELP("Move cursor to the opening curly brace in a block")
		movement, { .i = VIS_MOVE_BLOCK_START }
	},
	{
		"vis-motion-block-end",
		VIS_HELP("Move cursor to the closing curly brace in a block")
		movement, { .i = VIS_MOVE_BLOCK_END }
	},
	{
		"vis-motion-parenthesis-start",
		VIS_HELP("Move cursor to the opening parenthesis inside a pair of parentheses")
		movement, { .i = VIS_MOVE_PARENTHESIS_START }
	},
	{
		"vis-motion-parenthesis-end",
		VIS_HELP("Move cursor to the closing parenthesis inside a pair of parentheses")
		movement, { .i = VIS_MOVE_PARENTHESIS_END }
	},
	{
		"vis-motion-column",
		VIS_HELP("Move cursor to given column of current line")
		movement, { .i = VIS_MOVE_COLUMN }
	},
	{
		"vis-motion-line-first",
		VIS_HELP("Move cursor to given line (defaults to first)")
		gotoline, { .i = -1 }
	},
	{
		"vis-motion-line-last",
		VIS_HELP("Move cursor to given line (defaults to last)")
		gotoline, { .i = +1 }
	},
	{
		"vis-motion-window-line-top",
		VIS_HELP("Move cursor to top line of the window")
		movement, { .i = VIS_MOVE_WINDOW_LINE_TOP }
	},
	{
		"vis-motion-window-line-middle",
		VIS_HELP("Move cursor to middle line of the window")
		movement, { .i = VIS_MOVE_WINDOW_LINE_MIDDLE }
	},
	{
		"vis-motion-window-line-bottom",
		VIS_HELP("Move cursor to bottom line of the window")
		movement, { .i = VIS_MOVE_WINDOW_LINE_BOTTOM }
	},
	{
		"vis-motion-search-repeat-forward",
		VIS_HELP("Move cursor to next match in forward direction")
		movement, { .i = VIS_MOVE_SEARCH_REPEAT_FORWARD }
	},
	{
		"vis-motion-search-repeat-backward",
		VIS_HELP("Move cursor to previous match in backward direction")
		movement, { .i = VIS_MOVE_SEARCH_REPEAT_BACKWARD }
	},
	{
		"vis-motion-search-repeat",
		VIS_HELP("Move cursor to next match")
		movement, { .i = VIS_MOVE_SEARCH_REPEAT }
	},
	{
		"vis-motion-search-repeat-reverse",
		VIS_HELP("Move cursor to next match in opposite direction")
		movement, { .i = VIS_MOVE_SEARCH_REPEAT_REVERSE }
	},
	{
		"vis-motion-search-word-forward",
		VIS_HELP("Move cursor to next occurrence of the word under cursor")
		movement, { .i = VIS_MOVE_SEARCH_WORD_FORWARD }
	},
	{
		"vis-motion-search-word-backward",
		VIS_HELP("Move cursor to previous occurrence of the word under cursor")
		movement, { .i = VIS_MOVE_SEARCH_WORD_BACKWARD }
	},
	{
		"vis-window-page-up",
		VIS_HELP("Scroll window pages backwards (upwards)")
		wscroll, { .i = -PAGE }
	},
	{
		"vis-window-halfpage-up",
		VIS_HELP("Scroll window half pages backwards (upwards)")
		wscroll, { .i = -PAGE_HALF }
	},
	{
		"vis-window-page-down",
		VIS_HELP("Scroll window pages forwards (downwards)")
		wscroll, { .i = +PAGE }
	},
	{
		"vis-window-halfpage-down",
		VIS_HELP("Scroll window half pages forwards (downwards)")
		wscroll, { .i = +PAGE_HALF }
	},
	{
		"vis-mode-normal",
		VIS_HELP("Enter normal mode")
		switchmode, { .i = VIS_MODE_NORMAL }
	},
	{
		"vis-mode-normal-escape",
		VIS_HELP("Reset count or remove all non-primary selections")
		normalmode_escape,
	},
	{
		"vis-mode-visual-charwise",
		VIS_HELP("Enter characterwise visual mode")
		switchmode, { .i = VIS_MODE_VISUAL }
	},
	{
		"vis-mode-visual-escape",
		VIS_HELP("Reset count or switch to normal mode")
		visualmode_escape,
	},
	{
		"vis-mode-visual-linewise",
		VIS_HELP("Enter linewise visual mode")
		switchmode, { .i = VIS_MODE_VISUAL_LINE }
	},
	{
		"vis-mode-insert",
		VIS_HELP("Enter insert mode")
		insertmode, { .i = VIS_MOVE_NOP }
	},
	{
		"vis-mode-replace",
		VIS_HELP("Enter replace mode")
		replacemode, { .i = VIS_MOVE_NOP }
	},
	{
		"vis-delete-char-prev",
		VIS_HELP("Delete the previous character")
		delete, { .i = VIS_MOVE_CHAR_PREV }
	},
	{
		"vis-delete-char-next",
		VIS_HELP("Delete the next character")
		delete, { .i = VIS_MOVE_CHAR_NEXT }
	},
	{
		"vis-delete-line-begin",
		VIS_HELP("Delete until the start of the current line")
		delete, { .i = VIS_MOVE_LINE_BEGIN }
	},
	{
		"vis-delete-word-prev",
		VIS_HELP("Delete the previous WORD")
		delete, { .i = VIS_MOVE_WORD_START_PREV }
	},
	{
		"vis-jumplist-prev",
		VIS_HELP("Go to older cursor position in jump list")
		jumplist, { .i = -1 }
	},
	{
		"vis-jumplist-next",
		VIS_HELP("Go to newer cursor position in jump list")
		jumplist, { .i = +1 }
	},
	{
		"vis-jumplist-save",
		VIS_HELP("Save current selections in jump list")
		jumplist, { .i = 0 }
	},
	{
		"vis-undo",
		VIS_HELP("Undo last change")
		undo,
	},
	{
		"vis-redo",
		VIS_HELP("Redo last change")
		redo,
	},
	{
		"vis-earlier",
		VIS_HELP("Goto older text state")
		earlier,
	},
	{
		"vis-later",
		VIS_HELP("Goto newer text state")
		later,
	},
	{
		"vis-macro-record",
		VIS_HELP("Record macro into given register")
		macro_record,
	},
	{
		"vis-macro-replay",
		VIS_HELP("Replay macro, execute the content of the given register")
		macro_replay,
	},
	{
		"vis-mark",
		VIS_HELP("Use given mark for next action")
		mark,
	},
	{
		"vis-redraw",
		VIS_HELP("Redraw current editor content")
		call, { .f = vis_redraw }
	},
	{
		"vis-replace-char",
		VIS_HELP("Replace the character under the cursor")
		replace,
	},
	{
		"vis-motion-totill-repeat",
		VIS_HELP("Repeat latest to/till motion")
		movement, { .i = VIS_MOVE_TOTILL_REPEAT }
	},
	{
		"vis-motion-totill-reverse",
		VIS_HELP("Repeat latest to/till motion but in opposite direction")
		movement, { .i = VIS_MOVE_TOTILL_REVERSE }
	},
	{
		"vis-search-forward",
		VIS_HELP("Search forward")
		prompt_show, { .s = "/" }
	},
	{
		"vis-search-backward",
		VIS_HELP("Search backward")
		prompt_show, { .s = "?" }
	},
	{
		"vis-motion-till-left",
		VIS_HELP("Till after the occurrence of character to the left")
		movement_key, { .i = VIS_MOVE_TILL_LEFT }
	},
	{
		"vis-motion-till-right",
		VIS_HELP("Till before the occurrence of character to the right")
		movement_key, { .i = VIS_MOVE_TILL_RIGHT }
	},
	{
		"vis-motion-till-line-left",
		VIS_HELP("Till after the occurrence of character to the left on the current line")
		movement_key, { .i = VIS_MOVE_TILL_LINE_LEFT }
	},
	{
		"vis-motion-till-line-right",
		VIS_HELP("Till before the occurrence of character to the right on the current line")
		movement_key, { .i = VIS_MOVE_TILL_LINE_RIGHT }
	},
	{
		"vis-motion-to-left",
		VIS_HELP("To the first occurrence of character to the left")
		movement_key, { .i = VIS_MOVE_TO_LEFT }
	},
	{
		"vis-motion-to-right",
		VIS_HELP("To the first occurrence of character to the right")
		movement_key, { .i = VIS_MOVE_TO_RIGHT }
	},
	{
		"vis-motion-to-line-left",
		VIS_HELP("To the first occurrence of character to the left on the current line")
		movement_key, { .i = VIS_MOVE_TO_LINE_LEFT }
	},
	{
		"vis-motion-to-line-right",
		VIS_HELP("To the first occurrence of character to the right on the current line")
		movement_key, { .i = VIS_MOVE_TO_LINE_RIGHT }
	},
	{
		"vis-register",
		VIS_HELP("Use given register for next operator")
		reg,
	},
	{
		"vis-operator-change",
		VIS_HELP("Change operator")
		operator, { .i = VIS_OP_CHANGE }
	},
	{
		"vis-operator-delete",
		VIS_HELP("Delete operator")
		operator, { .i = VIS_OP_DELETE }
	},
	{
		"vis-operator-yank",
		VIS_HELP("Yank operator")
		operator, { .i = VIS_OP_YANK }
	},
	{
		"vis-operator-shift-left",
		VIS_HELP("Shift left operator")
		operator, { .i = VIS_OP_SHIFT_LEFT }
	},
	{
		"vis-operator-shift-right",
		VIS_HELP("Shift right operator")
		operator, { .i = VIS_OP_SHIFT_RIGHT }
	},
	{
		"vis-count-zero",
		VIS_HELP("Count specifier")
		count, { .i = 0 }
	},
	{
		"vis-count-one",
		VIS_HELP("Count specifier")
		count, { .i = 1 }
	},
	{
		"vis-count-two",
		VIS_HELP("Count specifier")
		count, { .i = 2 }
	},
	{
		"vis-count-three",
		VIS_HELP("Count specifier")
		count, { .i = 3 }
	},
	{
		"vis-count-four",
		VIS_HELP("Count specifier")
		count, { .i = 4 }
	},
	{
		"vis-count-five",
		VIS_HELP("Count specifier")
		count, { .i = 5 }
	},
	{
		"vis-count-six",
		VIS_HELP("Count specifier")
		count, { .i = 6 }
	},
	{
		"vis-count-seven",
		VIS_HELP("Count specifier")
		count, { .i = 7 }
	},
	{
		"vis-count-eight",
		VIS_HELP("Count specifier")
		count, { .i = 8 }
	},
	{
		"vis-count-nine",
		VIS_HELP("Count specifier")
		count, { .i = 9 }
	},
	{
		"vis-insert-newline",
		VIS_HELP("Insert a line break (depending on file type)")
		call, { .f = vis_insert_nl }
	},
	{
		"vis-insert-tab",
		VIS_HELP("Insert a tab (might be converted to spaces)")
		call, { .f = vis_insert_tab }
	},
	{
		"vis-insert-verbatim",
		VIS_HELP("Insert Unicode character based on code point")
		insert_verbatim,
	},
	{
		"vis-insert-register",
		VIS_HELP("Insert specified register content")
		insert_register,
	},
	{
		"vis-window-next",
		VIS_HELP("Focus next window")
		call, { .f = vis_window_next }
	},
	{
		"vis-window-prev",
		VIS_HELP("Focus previous window")
		call, { .f = vis_window_prev }
	},
	{
		"vis-append-char-next",
		VIS_HELP("Append text after the cursor")
		insertmode, { .i = VIS_MOVE_LINE_CHAR_NEXT }
	},
	{
		"vis-append-line-end",
		VIS_HELP("Append text after the end of the line")
		insertmode, { .i = VIS_MOVE_LINE_END },
	},
	{
		"vis-insert-line-start",
		VIS_HELP("Insert text before the first non-blank in the line")
		insertmode, { .i = VIS_MOVE_LINE_START },
	},
	{
		"vis-open-line-above",
		VIS_HELP("Begin a new line above the cursor")
		openline, { .i = -1 }
	},
	{
		"vis-open-line-below",
		VIS_HELP("Begin a new line below the cursor")
		openline, { .i = +1 }
	},
	{
		"vis-join-lines",
		VIS_HELP("Join selected lines")
		join, { .s = " " }
	},
	{
		"vis-join-lines-trim",
		VIS_HELP("Join selected lines, remove white space")
		join, { .s = "" }
	},
	{
		"vis-prompt-show",
		VIS_HELP("Show editor command line prompt")
		prompt_show, { .s = ":" }
	},
	{
		"vis-repeat",
		VIS_HELP("Repeat latest editor command")
		repeat
	},
	{
		"vis-selection-flip",
		VIS_HELP("Flip selection, move cursor to other end")
		selection_end,
	},
	{
		"vis-window-redraw-top",
		VIS_HELP("Redraw cursor line at the top of the window")
		window, { .w = view_redraw_top }
	},
	{
		"vis-window-redraw-center",
		VIS_HELP("Redraw cursor line at the center of the window")
		window, { .w = view_redraw_center }
	},
	{
		"vis-window-redraw-bottom",
		VIS_HELP("Redraw cursor line at the bottom of the window")
		window, { .w = view_redraw_bottom }
	},
	{
		"vis-window-slide-up",
		VIS_HELP("Slide window content upwards")
		wslide, { .i = -1 }
	},
	{
		"vis-window-slide-down",
		VIS_HELP("Slide window content downwards")
		wslide, { .i = +1 }
	},
	{
		"vis-put-after",
		VIS_HELP("Put text after the cursor")
		operator, { .i = VIS_OP_PUT_AFTER }
	},
	{
		"vis-put-before",
		VIS_HELP("Put text before the cursor")
		operator, { .i = VIS_OP_PUT_BEFORE }
	},
	{
		"vis-selection-new-lines-above",
		VIS_HELP("Create a new selection on the line above")
		selections_new, { .i = -1 }
	},
	{
		"vis-selection-new-lines-above-first",
		VIS_HELP("Create a new selection on the line above the first selection")
		selections_new, { .i = INT_MIN }
	},
	{
		"vis-selection-new-lines-below",
		VIS_HELP("Create a new selection on the line below")
		selections_new, { .i = +1 }
	},
	{
		"vis-selection-new-lines-below-last",
		VIS_HELP("Create a new selection on the line below the last selection")
		selections_new, { .i = INT_MAX }
	},
	{
		"vis-selection-new-lines-begin",
		VIS_HELP("Create a new selection at the start of every line covered by selection")
		operator, { .i = VIS_OP_CURSOR_SOL }
	},
	{
		"vis-selection-new-lines-end",
		VIS_HELP("Create a new selection at the end of every line covered by selection")
		operator, { .i = VIS_OP_CURSOR_EOL }
	},
	{
		"vis-selection-new-match-all",
		VIS_HELP("Select all regions matching the current selection")
		selections_match_next, { .b = true }
	},
	{
		"vis-selection-new-match-next",
		VIS_HELP("Select the next region matching the current selection")
		selections_match_next,
	},
	{
		"vis-selection-new-match-skip",
		VIS_HELP("Clear current selection, but select next match")
		selections_match_skip,
	},
	{
		"vis-selections-align",
		VIS_HELP("Try to align all selections on the same column")
		selections_align,
	},
	{
		"vis-selections-align-indent-left",
		VIS_HELP("Left-align all selections by inserting spaces")
		selections_align_indent, { .i = -1 }
	},
	{
		"vis-selections-align-indent-right",
		VIS_HELP("Right-align all selections by inserting spaces")
		selections_align_indent, { .i = +1 }
	},
	{
		"vis-selections-remove-all",
		VIS_HELP("Remove all but the primary selection")
		selections_clear,
	},
	{
		"vis-selections-remove-last",
		VIS_HELP("Remove primary selection")
		selections_remove,
	},
	{
		"vis-selections-remove-column",
		VIS_HELP("Remove count selection column")
		selections_remove_column, { .i = 1 }
	},
	{
		"vis-selections-remove-column-except",
		VIS_HELP("Remove all but the count selection column")
		selections_remove_column_except, { .i = 1 }
	},
	{
		"vis-selection-prev",
		VIS_HELP("Move to the previous selection")
		selections_navigate, { .i = -PAGE_HALF }
	},
	{
		"vis-selection-next",
		VIS_HELP("Move to the next selection")
		selections_navigate, { .i = +PAGE_HALF }
	},
	{
		"vis-selections-rotate-left",
		VIS_HELP("Rotate selections left")
		selections_rotate, { .i = -1 }
	},
	{
		"vis-selections-rotate-right",
		VIS_HELP("Rotate selections right")
		selections_rotate, { .i = +1 }
	},
	{
		"vis-selections-trim",
		VIS_HELP("Remove leading and trailing white space from selections")
		selections_trim
	},
	{
		"vis-selections-save",
		VIS_HELP("Save currently active selections to mark")
		selections_save
	},
	{
		"vis-selections-restore",
		VIS_HELP("Restore selections from mark")
		selections_restore
	},
	{
		"vis-selections-union",
		VIS_HELP("Add selections from mark")
		selections_union
	},
	{
		"vis-selections-intersect",
		VIS_HELP("Intersect with selections from mark")
		selections_intersect
	},
	{
		"vis-selections-complement",
		VIS_HELP("Complement selections")
		selections_complement
	},
	{
		"vis-selections-minus",
		VIS_HELP("Subtract selections from mark")
		selections_minus
	},
	{
		"vis-textobject-word-outer",
		VIS_HELP("A word leading and trailing whitespace included")
		textobj, { .i = VIS_TEXTOBJECT_OUTER_WORD }
	},
	{
		"vis-textobject-word-inner",
		VIS_HELP("A word leading and trailing whitespace excluded")
		textobj, { .i = VIS_TEXTOBJECT_INNER_WORD }
	},
	{
		"vis-textobject-bigword-outer",
		VIS_HELP("A WORD leading and trailing whitespace included")
		textobj, { .i = VIS_TEXTOBJECT_OUTER_LONGWORD }
	},
	{
		"vis-textobject-bigword-inner",
		VIS_HELP("A WORD leading and trailing whitespace excluded")
		textobj, { .i = VIS_TEXTOBJECT_INNER_LONGWORD }
	},
	{
		"vis-textobject-sentence",
		VIS_HELP("A sentence")
		textobj, { .i = VIS_TEXTOBJECT_SENTENCE }
	},
	{
		"vis-textobject-paragraph",
		VIS_HELP("A paragraph")
		textobj, { .i = VIS_TEXTOBJECT_PARAGRAPH }
	},
	{
		"vis-textobject-paragraph-outer",
		VIS_HELP("A paragraph (outer variant)")
		textobj, { .i = VIS_TEXTOBJECT_PARAGRAPH_OUTER }
	},
	{
		"vis-textobject-square-bracket-outer",
		VIS_HELP("ock (outer variant)")
		textobj, { .i = VIS_TEXTOBJECT_OUTER_SQUARE_BRACKET }
	},
	{
		"vis-textobject-square-bracket-inner",
		VIS_HELP("ock (inner variant)")
		textobj, { .i = VIS_TEXTOBJECT_INNER_SQUARE_BRACKET }
	},
	{
		"vis-textobject-parenthesis-outer",
		VIS_HELP("() block (outer variant)")
		textobj, { .i = VIS_TEXTOBJECT_OUTER_PARENTHESIS }
	},
	{
		"vis-textobject-parenthesis-inner",
		VIS_HELP("() block (inner variant)")
		textobj, { .i = VIS_TEXTOBJECT_INNER_PARENTHESIS }
	},
	{
		"vis-textobject-angle-bracket-outer",
		VIS_HELP("<> block (outer variant)")
		textobj, { .i = VIS_TEXTOBJECT_OUTER_ANGLE_BRACKET }
	},
	{
		"vis-textobject-angle-bracket-inner",
		VIS_HELP("<> block (inner variant)")
		textobj, { .i = VIS_TEXTOBJECT_INNER_ANGLE_BRACKET }
	},
	{
		"vis-textobject-curly-bracket-outer",
		VIS_HELP("{} block (outer variant)")
		textobj, { .i = VIS_TEXTOBJECT_OUTER_CURLY_BRACKET }
	},
	{
		"vis-textobject-curly-bracket-inner",
		VIS_HELP("{} block (inner variant)")
		textobj, { .i = VIS_TEXTOBJECT_INNER_CURLY_BRACKET }
	},
	{
		"vis-textobject-quote-outer",
		VIS_HELP("A quoted string, including the quotation marks")
		textobj, { .i = VIS_TEXTOBJECT_OUTER_QUOTE }
	},
	{
		"vis-textobject-quote-inner",
		VIS_HELP("A quoted string, excluding the quotation marks")
		textobj, { .i = VIS_TEXTOBJECT_INNER_QUOTE }
	},
	{
		"vis-textobject-single-quote-outer",
		VIS_HELP("A single quoted string, including the quotation marks")
		textobj, { .i = VIS_TEXTOBJECT_OUTER_SINGLE_QUOTE }
	},
	{
		"vis-textobject-single-quote-inner",
		VIS_HELP("A single quoted string, excluding the quotation marks")
		textobj, { .i = VIS_TEXTOBJECT_INNER_SINGLE_QUOTE }
	},
	{
		"vis-textobject-backtick-outer",
		VIS_HELP("A backtick delimited string (outer variant)")
		textobj, { .i = VIS_TEXTOBJECT_OUTER_BACKTICK }
	},
	{
		"vis-textobject-backtick-inner",
		VIS_HELP("A backtick delimited string (inner variant)")
		textobj, { .i = VIS_TEXTOBJECT_INNER_BACKTICK }
	},
	{
		"vis-textobject-line-outer",
		VIS_HELP("The whole line")
		textobj, { .i = VIS_TEXTOBJECT_OUTER_LINE }
	},
	{
		"vis-textobject-line-inner",
		VIS_HELP("The whole line, excluding leading and trailing whitespace")
		textobj, { .i = VIS_TEXTOBJECT_INNER_LINE }
	},
	{
		"vis-textobject-indentation",
		VIS_HELP("All adjacent lines with the same indentation level as the current one")
		textobj, { .i = VIS_TEXTOBJECT_INDENTATION }
	},
	{
		"vis-textobject-search-forward",
		VIS_HELP("The next search match in forward direction")
		textobj, { .i = VIS_TEXTOBJECT_SEARCH_FORWARD }
	},
	{
		"vis-textobject-search-backward",
		VIS_HELP("The next search match in backward direction")
		textobj, { .i = VIS_TEXTOBJECT_SEARCH_BACKWARD }
	},
	{
		"vis-unicode-info",
		VIS_HELP("Show Unicode codepoint(s) of character under cursor")
		unicode_info, { .s = "a"  }
	},
	{
		"vis-utf8-info",
		VIS_HELP("Show UTF-8 encoded codepoint(s) of character under cursor")
		unicode_info, { .s = "8" }
	},
	{
		"vis-nop",
		VIS_HELP("Ignore key, do nothing")
		nop,
	},
};
