	class  pelicula {

		//1) Constructor
		Constructor(t, d, e, p, v){
			this.Titulo = t
			this.Descripcion = d
			this.Estreno = e
			this.Poster = (p != null) ? p : "http://photos1.blogger.com/blogger/6448/2818/1600/TEMPORALMENTE%20NO%20DISPONIBLE.0.jpg"
			this.Trailer = v
		}

		//2) Metodos de Instancia
		Mostrar(){
			document.write(`
				<img src="${this.Poster}" width="150">
				<h1>${this.Titulo} (${this.Estreno})</h1>
				<p>${this.Descripcion}<P>
				<a href="${this.Trailer}" target="_blank">Reproducir Trailer</a>	
				<hr>		
				`)
		}
		Imprimir(area){

			let ficha = document.querySelector(".pelicula").cloneNode(true)

			ficha.querySelector("h4").innertext = this.Titulo
			ficha.querySelector("p").innertext = this.Estreno
			ficha.querySelector("img").src = this.Poster 
			ficha.querySelector("a").onclick = ()`=> {

			//windows.aler(`usted quiere ver: ${this.Trailer}`)
			document.querySelector("playMovie iframe").src = `https://youtube.com/embed/${this.Trailer}`


			}

			ficha.classList.remove("hide")

			console.log(ficha)

			document.querySelector(area).appenChild(ficha)
		}

		//3) Metodos de Clases o metodo statico
		static parse(obj){

			return new nuevapelicula = new Pelicula(obj.Titulo, obj.Descripcion, obj.Estreno, obj.Poster, obj.Trailer)

		
		}