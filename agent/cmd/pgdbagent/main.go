/***************************************************************************
 *  PGDBATOOLS AGENT
 *
 *  START  : 2026-08-06
 *  AUTHOR : Marco Túlio Castro
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

package main

import (
	"context"
	"flag"
	"pgdbagent/internal/config"
	"pgdbagent/internal/db"

	//"fmt"
	"log"
	//"github.com/jackc/pgx/v5"
)

func main() {

	ctx := context.Background()
	configPath := flag.String("config", "config.yaml", "Configuration File")
	flag.Parse()

	cfg, err := config.Load(*configPath)
	if err != nil {
		log.Fatal(err)
	}
	if err := cfg.Validate(); err != nil {
		log.Fatal(err)
	}
	if cfg != nil {
		_ = db.Connect(ctx, cfg)
	}
}
